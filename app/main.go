package main

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/constants"
	"cycleSenseCentral/src/input"
	"cycleSenseCentral/src/sensors/bike"
	"cycleSenseCentral/src/sensors/chuck"
	"cycleSenseCentral/src/sensors/manager"
	"cycleSenseCentral/src/sensors/sensor_event"
	"cycleSenseCentral/src/web"
	"cycleSenseCentral/src/wifi"
)

var senseController input.SenseController

func main() {
	go web.RunWebserver()

	// Sensor events channel
	sensorsEventChan := make(chan sensor_event.SensorEvent)

	// Initialize CycleSense virtual controller
	input.NewSenseController(sensorsEventChan)
	input.Input.Initialize()

	// Initialize Sensor Manager singleton
	manager.NewSensorDataManager()

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
		ble.NewBLEHandler(constants.BT_MAC_ADDR, []ble.BLESubscription{
			btbike.GetBLESubscription(),
			btchuck.GetBLESubscription(),
		})

		// Initialize BLE and subscribe to requested services/characteristics
		ble.BLE.InitializeBluetooth()

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

		wifi.NewWifiHandler()
		wifi.Handler.InitializeWifi()

		wifibike.InitializeHandlers()
		wifichuck.InitializeHandlers()
		break
	}

	select {}
}
