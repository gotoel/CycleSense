package main

import (
	"encoding/binary"
	"fmt"
	"math"
	"time"
	"tinygo.org/x/bluetooth"
)

var adapter = bluetooth.DefaultAdapter

const (
	CS_DEVICE_ADDR = "34:94:54:27:6A:BE"

	JOB_SERVICE_UUID = 0x1337
	RPM_UUID         = 0x1338
)

func main() {
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
	srvcs, err := device.DiscoverServices([]bluetooth.UUID{bluetooth.New16BitUUID(JOB_SERVICE_UUID)})
	must("discover services", err)

	if len(srvcs) == 0 {
		panic("could not find heart rate service")
	}

	srvc := srvcs[0]

	println("found service", srvc.UUID().String())

	chars, err := srvc.DiscoverCharacteristics([]bluetooth.UUID{bluetooth.New16BitUUID(RPM_UUID)})
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
		println(fmt.Sprintf("RPMs: %.2f", float))
	})

	select {}
}

func must(action string, err error) {
	if err != nil {
		panic("failed to " + action + ": " + err.Error())
	}
}
