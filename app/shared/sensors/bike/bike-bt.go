package bike

import (
	"encoding/binary"
	"math"
	"shared/comms"
	"shared/sensors/sensor"
)

const (
	BikeServiceUuid = "97bb6403-1337-4a42-8563-243ed61234c7"
	RpmUuid         = "97bb6403-1338-4a42-8563-243ed61234c7"
)

type BTBikeSensor struct {
	sensor.Sensor
	sensor.SensorBT
	Data *Data
}

func (bike *BTBikeSensor) Initialize() {
	bike.setHandlers()
}

func (bike *BTBikeSensor) setHandlers() {
	// Set RPM handler
	comms.BLE.SetNotificationHandler(RpmUuid, func(buf []byte) {
		bits := binary.LittleEndian.Uint32(buf)
		float := math.Float32frombits(bits)
		bike.Data.RPM = float
	})
}

func (bike *BTBikeSensor) GetBLESubscription() (subscription comms.BLESubscription) {
	subscription = comms.BLESubscription{
		Name:        Name,
		ServiceUUID: BikeServiceUuid,
		CharacteristicUUIDs: []string{
			RpmUuid,
		},
	}
	return
}
