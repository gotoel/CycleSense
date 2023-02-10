package main

import (
	"shared/comms"
	"shared/constants"
	"shared/input"
	"shared/sensors/bike"
	"shared/sensors/chuck"
	"shared/sensors/manager"
	"shared/types"
	"shared/web"
)

var senseController input.SenseController

func main() {
	go web.RunWebserver()

	// Sensor events channel
	sensorsEventChan := make(chan types.Event)

	// Initialize CycleSense virtual controller
	input.NewSenseController(sensorsEventChan)
	input.Input.Initialize()

	// Initialize Sensor Manager singleton
	manager.NewSensorDataManager(sensorsEventChan)

	switch constants.DEVICE_CONNECTION_TYPE {
	case constants.Bluetooth:
		btbike := bike.BTBikeSensor{}
		btchuck := chuck.BTChuckSensor{}

		btbike.SetEventsChannel(sensorsEventChan)
		btchuck.SetEventsChannel(sensorsEventChan)

		manager.Manager.Bike = &btbike.Data
		manager.Manager.Chuck = &btchuck.Data

		// All used services/characteristics need to be
		// added here for subscriptions.
		comms.NewBLEHandler(constants.BtMacAddr, []comms.BLESubscription{
			btbike.GetBLESubscription(),
			btchuck.GetBLESubscription(),
		})

		// Initialize BLE and subscribe to requested services/characteristics
		comms.BLE.InitializeBluetooth()

		// Must initialize BT sensors after bluetooth is initialized
		btbike.Initialize()
		btchuck.Initialize()
		break
	case constants.WiFi:
		wifibike := bike.WifiBikeSensor{}
		wifichuck := chuck.WifiChuckSensor{}

		wifibike.SetEventsChannel(sensorsEventChan)
		wifichuck.SetEventsChannel(sensorsEventChan)

		manager.Manager.Bike = &wifibike.Data
		manager.Manager.Chuck = &wifichuck.Data

		comms.NewWifiHandler(sensorsEventChan)
		comms.Handler.InitializeWifi()

		wifibike.InitializeHandlers()
		wifichuck.InitializeHandlers()
		break
	}

	select {}
}
