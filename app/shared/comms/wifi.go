package comms

import (
	"encoding/json"
	"fmt"
	"github.com/vmihailenco/msgpack/v5"
	"net"
	"shared/constants"
	"shared/types"
	"sync"
	"time"
)

var onceWifi sync.Once

const (
	WifiConnectMsg = "CYCLESENSE|CONNECT"

	ConnectAckHandler = "ConnectAck"

	Heartbeat                 = "Heartbeat"
	HeartbeatIntervalSeconds  = 2
	HeartbeatFailureThreshold = 3
)

type HeartbeatResponse struct {
	Msg string `msgpack:"msg"`
}

type ConnectAck struct {
	Msg string `msgpack:"msg"`
}

type WiFiHandler struct {
	addr *net.UDPAddr
	conn *net.UDPConn

	connected       bool
	heartbeatQueued int

	handlers map[string]func(data []byte)

	eventChannel chan<- types.Event
}

var (
	Handler *WiFiHandler
)

func NewWifiHandler(eventsChan chan<- types.Event) *WiFiHandler {
	onceWifi.Do(func() {
		Handler = &WiFiHandler{
			eventChannel: eventsChan,
			handlers:     make(map[string]func(data []byte)),
		}
	})

	return Handler
}

func (wifi *WiFiHandler) InitializeWifi() {
	var err error
	wifi.addr, err = net.ResolveUDPAddr(
		"udp", fmt.Sprintf("%s:%d", constants.DeviceWifiIp, constants.DeviceWifiPort))

	if err != nil {
		println("ResolveUDPAddr failed:", err.Error())
	}

	wifi.conn, err = net.DialUDP("udp", nil, wifi.addr)
	if err != nil {
		println("Listen failed:", err.Error())
	}

	// Register connection acknowledgement message handler
	// this should probably be more secure...
	wifi.AddHandler(ConnectAckHandler, func(data []byte) {
		msg := string(data)

		if err == nil {
			log(fmt.Sprintf("Connected to CycleSense device, msg from device: %s", msg))
			wifi.connected = true

			go wifi.heartbeat()
		}
	})

	go wifi.attemptConnection()
	go wifi.listen()
}

func (wifi *WiFiHandler) IsConnected() bool {
	return wifi.connected
}

func (wifi *WiFiHandler) AddHandler(handlerName string, handlerFunc func(data []byte)) {
	wifi.handlers[handlerName] = handlerFunc
}

func (wifi *WiFiHandler) RemoveHandler(handlerName string) {
	if _, ok := wifi.handlers[handlerName]; ok {
		delete(wifi.handlers, handlerName)
	}
}

func (wifi *WiFiHandler) attemptConnection() {
	log("Attempting to connect to device...")
	for !wifi.connected {
		wifi.sendData([]byte(WifiConnectMsg))
		time.Sleep(time.Second * 2)
	}
}

func (wifi *WiFiHandler) heartbeat() {
	wifi.AddHandler(Heartbeat, func(data []byte) {
		wifi.heartbeatQueued = 0
	})

	for wifi.connected {
		// Exceeded the amount of heartbeats sent without
		// a response, disconnect.
		if wifi.heartbeatQueued > HeartbeatFailureThreshold {
			log("Heartbeat timed out, disconnecting from device...")
			wifi.RemoveHandler(Heartbeat)
			wifi.connected = false

			// Reset sensors on wifi disconnect
			wifi.eventChannel <- types.Event{Name: constants.EventReset}

			// Reset heartbeats
			wifi.heartbeatQueued = 0

			// Start trying to reconnect immediately
			go wifi.attemptConnection()
			break
		}

		wifi.sendData([]byte(Heartbeat))
		wifi.heartbeatQueued++
		time.Sleep(time.Second * HeartbeatIntervalSeconds)
	}
}

func (wifi *WiFiHandler) listen() {
	defer wifi.conn.Close()

	for {
		// buffer to get data
		buffer := make([]byte, 1024)
		readLength, err := wifi.conn.Read(buffer)
		if err != nil {
			println("Read data failed:", err.Error())
		}

		data := buffer[0:readLength]
		//println(string(data))
		wifi.processUdpData(data)
	}
}

func (wifi *WiFiHandler) sendData(data []byte) {
	_, err := wifi.conn.Write(data)
	if err != nil {
		println("Write data failed:", err.Error())
	}
}

func (wifi *WiFiHandler) processUdpData(udpData []byte) {
	dataMap := make(map[string]interface{})
	err := msgpack.Unmarshal(udpData, &dataMap)
	if err == nil {
		if typeName, ok := dataMap["type"]; ok {
			formattedTypeName := fmt.Sprintf("%v", typeName)
			if handlerFunc, ok2 := wifi.handlers[formattedTypeName]; ok2 {
				if packetData, dataOk := dataMap["data"]; dataOk {
					data, _ := json.Marshal(packetData)
					handlerFunc(data)
				} else {
					handlerFunc([]byte{})
				}
			}
		}
	}
}

func log(str string) {
	println(fmt.Sprintf("[%s] %s", time.Now().Format(time.Stamp), str))
}
