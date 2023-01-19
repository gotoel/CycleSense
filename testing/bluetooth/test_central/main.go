package main

import (
	"encoding/binary"
	"fmt"
	"math"
	"time"
	"tinygo.org/x/bluetooth"
)

type BikeStats struct {
	RPM float32
}

var adapter = bluetooth.DefaultAdapter
var Bike BikeStats

const (
	CS_DEVICE_ADDR = "34:94:54:27:6A:BE"

	JOB_SERVICE_UUID = "97bb6403-1337-4a42-8563-243ed61234c7"
	RPM_UUID         = "97bb6403-1338-4a42-8563-243ed61234c7"
)

func main() {
	go runWebserver()

	// Enable BLE interface.
	must("enable BLE stack", adapter.Enable())

	ch := make(chan bluetooth.ScanResult, 1)

	// Start scanning.
	println("scanning...")
	err := adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
		if result.Address.String() == CS_DEVICE_ADDR {
			now := time.Now()
			println(now.Format(time.Stamp), " - found device:", result.Address.String(), result.RSSI, result.LocalName())
			adapter.StopScan()
			ch <- result
		}
	})

	var device *bluetooth.Device
	select {
	case result := <-ch:
		device, err = adapter.Connect(result.Address, bluetooth.ConnectionParams{})
		if err != nil {
			println(err.Error())
			return
		}

		println("connected to ", result.Address.String())
	}

	// get services
	println("discovering services/characteristics")
	uuid, _ := bluetooth.ParseUUID(JOB_SERVICE_UUID)
	srvcs, err := device.DiscoverServices([]bluetooth.UUID{uuid})
	must("discover services", err)

	if len(srvcs) == 0 {
		panic("could not find heart rate service")
	}

	srvc := srvcs[0]

	println("found service", srvc.UUID().String())

	uuid, _ = bluetooth.ParseUUID(RPM_UUID)
	chars, err := srvc.DiscoverCharacteristics([]bluetooth.UUID{uuid})
	if err != nil {
		println(err)
	}

	if len(chars) == 0 {
		panic("could not find heart rate characteristic")
	}

	char := chars[0]
	println("found characteristic", char.UUID().String())

	char.EnableNotifications(func(buf []byte) {
		bits := binary.LittleEndian.Uint32(buf)
		float := math.Float32frombits(bits)
		Bike.RPM = float
		println(fmt.Sprintf("RPMs: %.2f", float))
	})

	select {}
}

func must(action string, err error) {
	if err != nil {
		panic("failed to " + action + ": " + err.Error())
	}
}
