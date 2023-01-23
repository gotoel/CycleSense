package wifi

import (
	"cycleSenseCentral/src/constants"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"sync"
)

var once sync.Once

type WiFiHandler struct {
	addr *net.UDPAddr
	conn *net.UDPConn

	connected bool

	handlers map[string]func(buf []byte)
}

var (
	Handler *WiFiHandler
)

func NewWifiHandler() *WiFiHandler {
	once.Do(func() {
		Handler = &WiFiHandler{
			handlers: make(map[string]func(buf []byte)),
		}
	})

	return Handler
}

func (wifi *WiFiHandler) InitializeWifi() {
	var err error
	wifi.addr, err = net.ResolveUDPAddr(
		"udp", fmt.Sprintf("%s:%s", constants.DEVICE_WIFI_IP, "1337"))

	if err != nil {
		println("ResolveUDPAddr failed:", err.Error())
		os.Exit(1)
	}

	wifi.conn, err = net.DialUDP("udp", nil, wifi.addr)
	if err != nil {
		println("Listen failed:", err.Error())
		os.Exit(1)
	} else {
		wifi.connected = true
	}

	wifi.sendConnectMsg()

	go wifi.listen()
}

func (wifi *WiFiHandler) RegisterHandler(typeName string, handlerFunc func(data []byte)) {
	wifi.handlers[typeName] = handlerFunc
}

func (wifi *WiFiHandler) sendConnectMsg() {
	_, err := wifi.conn.Write([]byte(constants.WIFI_CONNECT_MSG))
	if err != nil {
		println("Write data failed:", err.Error())
		os.Exit(1)
	}
}

func (wifi *WiFiHandler) listen() {
	defer wifi.conn.Close()

	for wifi.connected {
		// buffer to get data
		buffer := make([]byte, 1024)
		readLength, err := wifi.conn.Read(buffer)
		if err != nil {
			println("Read data failed:", err.Error())
			os.Exit(1)
		}

		data := buffer[0:readLength]
		//println(string(data))
		wifi.processUdpData(string(data))
	}
}

func (wifi *WiFiHandler) processUdpData(data string) {
	sensorData := make(map[string]interface{})
	err := json.Unmarshal([]byte(data), &sensorData)
	if err == nil {
		if typeName, ok := sensorData["type"]; ok {
			formattedTypeName := fmt.Sprintf("%v", typeName)
			if handlerFunc, ok2 := wifi.handlers[formattedTypeName]; ok2 {
				handlerFunc([]byte(data))
			}
		}
	}
}
