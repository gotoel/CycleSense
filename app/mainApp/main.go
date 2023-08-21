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
	manager.NewSensorDataManager()

	manager.Manager.Bike = &bike.Data{}
	manager.Manager.Chuck = &chuck.Data{}

	switch constants.DEVICE_CONNECTION_TYPE {
	case constants.Bluetooth:
		// Set the data pointer on the sensors
		btbike := bike.BTBikeSensor{
			Data: manager.Manager.Bike,
		}
		btchuck := chuck.BTChuckSensor{
			Data: manager.Manager.Chuck,
		}

		btbike.SetEventsChannel(sensorsEventChan)
		btchuck.SetEventsChannel(sensorsEventChan)

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
		// Set the data pointer on the sensors
		wifibike := bike.WifiBikeSensor{
			Data: manager.Manager.Bike,
		}
		wifichuck := chuck.WifiChuckSensor{
			Data: manager.Manager.Chuck,
		}

		wifibike.SetEventsChannel(sensorsEventChan)
		wifichuck.SetEventsChannel(sensorsEventChan)

		comms.NewWifiHandler(sensorsEventChan)
		comms.Handler.InitializeWifi()

		wifibike.InitializeHandlers()
		wifichuck.InitializeHandlers()
		break
	}

	select {}
}
