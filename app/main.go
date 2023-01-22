package main

import (
	"cycleSenseCentral/src"
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/input"
	"cycleSenseCentral/src/sensors"
	"cycleSenseCentral/src/web"
)

var senseController input.SenseController

func main() {
	go web.RunWebserver()

	// Initialize CycleSense virtual controller
	input.NewSenseController()
	input.Input.Initialize()

	// Initialize Sensor Manager singleton
	sensors.NewSensorManager()

	// All used services/characteristics need to be
	// added here for subscriptions.
	ble.NewBLEHandler(src.CS_DEVICE_ADDR, []ble.BLESubscription{
		sensors.Manager.Bike.GetBLESubscription(),
		sensors.Manager.Chuck.GetBLESubscription(),
	})

	// Initialize BLE and subscribe to requested services/characteristics
	ble.BLE.InitializeBluetooth()

	// Sensors set BLE handlers, do this after initializing BLE
	sensors.Manager.InitializeSensors()

	select {}
}
