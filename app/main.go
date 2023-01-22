package main

import (
	"cycleSenseCentral/src"
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/input"
	"cycleSenseCentral/src/sensors"
	"cycleSenseCentral/src/sensors/bike"
	"cycleSenseCentral/src/sensors/chuck"
	"cycleSenseCentral/src/web"
)

var senseController input.SenseController

func main() {
	go web.RunWebserver()

	// Initialize CycleSense virtual controller
	input.NewSenseController()
	input.Input.Initialize()

	// Initialize Sensor Manager singleton
	sensors.NewSensorDataManager()

	switch src.DEVICE_CONNECTION_TYPE {
	case src.Bluetooth:
		btbike := bike.BTBikeSensor{}
		btchuck := chuck.BTChuckSensor{}

		sensors.Manager.Bike = &btbike.Data
		sensors.Manager.Chuck = &btchuck.Data

		// All used services/characteristics need to be
		// added here for subscriptions.
		ble.NewBLEHandler(src.CS_DEVICE_ADDR, []ble.BLESubscription{
			btbike.GetBLESubscription(),
			btchuck.GetBLESubscription(),
		})

		// Initialize BLE and subscribe to requested services/characteristics
		ble.BLE.InitializeBluetooth()

		// Must initialize BT sensors after bluetooth is initialized
		btbike.Initialize()
		btchuck.Initialize()
		break
	case src.WiFi:

		break

	}

	select {}
}
