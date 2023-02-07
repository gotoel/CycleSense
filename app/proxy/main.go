package main

import (
	"shared/comms"
	"shared/constants"
	"shared/input"
	"shared/sensors/manager"
)

var senseController input.SenseController

func main() {
	// Initialize Sensor Manager singleton
	manager.NewSensorDataManager()

	// Initialize wifi proxy
	NewWifiHandlerProxy()

	btbike := BTBikeSensorProxy{}
	btchuck := BTChuckSensorProxy{}

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

	Handler.InitializeWifi()

	// Must initialize BT sensors after bluetooth is initialized
	btbike.Initialize()
	btchuck.Initialize()

	select {}
}
