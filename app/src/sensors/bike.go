package sensors

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/input"
	"encoding/binary"
	"fmt"
	"math"
)

const (
	BikeServiceName = "Bike"

	BikeServiceUuid = "97bb6403-1337-4a42-8563-243ed61234c7"
	RpmUuid         = "97bb6403-1338-4a42-8563-243ed61234c7"
)

type BikeSensor struct {
	Sensor

	subscription ble.BLESubscription
	RPM          float32
}

func (bike *BikeSensor) initialize() {
	bike.setHandlers()
}

func (bike *BikeSensor) setHandlers() {
	// Set RPM handler
	ble.BLE.SetNotificationHandler(RpmUuid, func(buf []byte) {
		bits := binary.LittleEndian.Uint32(buf)
		float := math.Float32frombits(bits)
		bike.RPM = float

		input.Input.SetAxisRPM(bike.RPM)
		//const DEAD_ZONE = 30
		//if Bike.RPM > DEAD_ZONE {
		//	senseController.PressHotkey()
		//} else {
		//	senseController.ReleaseHotkey()
		//}
		println(fmt.Sprintf("RPMs: %.2f", float))
	})
}

func (bike *BikeSensor) GetBLESubscription() (subscription ble.BLESubscription) {
	subscription = ble.BLESubscription{
		Name:        BikeServiceName,
		ServiceUUID: BikeServiceUuid,
		CharacteristicUUIDs: []string{
			RpmUuid,
		},
	}
	return
}
