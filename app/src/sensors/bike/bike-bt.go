package bike

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/sensors/sensor"
	"encoding/binary"
	"math"
)

const (
	BikeServiceName = "Bike"

	BikeServiceUuid = "97bb6403-1337-4a42-8563-243ed61234c7"
	RpmUuid         = "97bb6403-1338-4a42-8563-243ed61234c7"
)

type BTBikeSensor struct {
	sensor.SensorBT
	Data Data
}

func (bike *BTBikeSensor) Initialize() {
	bike.Data = Data{}
	bike.setHandlers()
}

func (bike *BTBikeSensor) setHandlers() {
	// Set RPM handler
	ble.BLE.SetNotificationHandler(RpmUuid, func(buf []byte) {
		bits := binary.LittleEndian.Uint32(buf)
		float := math.Float32frombits(bits)
		bike.Data.RPM = float
		ProcessInputs(bike.Data)
		//println(fmt.Sprintf("RPMs: %.2f", float))
	})
}

func (bike *BTBikeSensor) GetBLESubscription() (subscription ble.BLESubscription) {
	subscription = ble.BLESubscription{
		Name:        BikeServiceName,
		ServiceUUID: BikeServiceUuid,
		CharacteristicUUIDs: []string{
			RpmUuid,
		},
	}
	return
}
