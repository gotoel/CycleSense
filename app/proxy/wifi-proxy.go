package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net"
	"shared/comms"
	"shared/constants"
	"sync"
)

var once sync.Once

type HeartbeatResponse struct {
	Msg string `msgpack:"msg"`
}

type ConnectAck struct {
	Msg string `msgpack:"msg"`
}

type WiFiHandlerProxy struct {
	currentIp net.Addr

	connected bool

	udpServer net.PacketConn
}

var (
	Handler *WiFiHandlerProxy
)

func NewWifiHandlerProxy() *WiFiHandlerProxy {
	once.Do(func() {
		Handler = &WiFiHandlerProxy{}
	})

	return Handler
}

func (wp *WiFiHandlerProxy) InitializeWifi() {
	var err error
	wp.udpServer, err = net.ListenPacket("udp", fmt.Sprintf(":%d", constants.DeviceWifiPort))
	if err != nil {
		log.Fatal(err)
	}
	defer wp.udpServer.Close()

	go wp.listen()
}

func (wp *WiFiHandlerProxy) SendData(data []byte) {
	if wp.currentIp != nil {
		_, err := wp.udpServer.WriteTo(data, wp.currentIp)
		if err != nil {
			println("Write data failed:", err.Error())
		}
	}
}

func (wp *WiFiHandlerProxy) listen() {
	for {
		buf := make([]byte, 1024)
		_, addr, err := wp.udpServer.ReadFrom(buf)
		if err != nil {
			continue
		}
		go wp.processResponse(wp.udpServer, addr, buf)
	}
}

func (wp *WiFiHandlerProxy) processResponse(udpServer net.PacketConn, addr net.Addr, buf []byte) {
	data := string(buf)

	if data == comms.Heartbeat {
		// Send heartbeat response
		udpServer.WriteTo([]byte(comms.Heartbeat), addr)
	} else if data == comms.WifiConnectMsg {
		// Send connection response
		wp.currentIp = addr
		hr := comms.HeartbeatResponse{
			Msg: "Hello, from the CycleSense Proxy....",
		}

		hrData, _ := json.Marshal(hr)
		wp.SendData(hrData)
	}
}
