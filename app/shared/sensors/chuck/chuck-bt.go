package chuck

import (
	"encoding/binary"
	"shared/comms"
	"shared/sensors/sensor"
	"shared/types"
)

const (
	ChuckServiceUuid = "97bb6403-0101-4a42-8563-243ed61234c7"
	ChuckAcclXUuid   = "97bb6403-0102-4a42-8563-243ed61234c7"
	ChuckAcclYUuid   = "97bb6403-0103-4a42-8563-243ed61234c7"
	ChuckAcclZUuid   = "97bb6403-0104-4a42-8563-243ed61234c7"
	ChuckAxisXUuid   = "97bb6403-0105-4a42-8563-243ed61234c7"
	ChuckAxisYUuid   = "97bb6403-0106-4a42-8563-243ed61234c7"
	ChuckButtonCUuid = "97bb6403-0107-4a42-8563-243ed61234c7"
	ChuckButtonZUuid = "97bb6403-0108-4a42-8563-243ed61234c7"
)

type BTChuckSensor struct {
	sensor.Sensor
	sensor.SensorBT
	Data *Data
}

func (chuck *BTChuckSensor) Initialize() {
	chuck.setHandlers()
}

func (chuck *BTChuckSensor) setHandlers() {
	// Acceleration (X,Y,Z)
	comms.BLE.SetNotificationHandler(ChuckAcclXUuid, func(buf []byte) {
		chuck.Data.Accelerometer.AcclX = binary.LittleEndian.Uint32(buf)
	})
	comms.BLE.SetNotificationHandler(ChuckAcclYUuid, func(buf []byte) {
		chuck.Data.Accelerometer.AcclY = binary.LittleEndian.Uint32(buf)
	})
	comms.BLE.SetNotificationHandler(ChuckAcclZUuid, func(buf []byte) {
		chuck.Data.Accelerometer.AcclZ = binary.LittleEndian.Uint32(buf)
	})

	// Thumb stick axis (X, Y)
	comms.BLE.SetNotificationHandler(ChuckAxisXUuid, func(buf []byte) {
		chuck.Data.Sticks.AxisLeftX = binary.LittleEndian.Uint32(buf)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})
	comms.BLE.SetNotificationHandler(ChuckAxisYUuid, func(buf []byte) {
		chuck.Data.Sticks.AxisLeftY = binary.LittleEndian.Uint32(buf)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	// Buttons: C, Z
	comms.BLE.SetNotificationHandler(ChuckButtonCUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.Data.Buttons.ButtonC = buf[0] == 1
		}
	})
	comms.BLE.SetNotificationHandler(ChuckButtonZUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.Data.Buttons.ButtonZ = buf[0] == 1
		}
	})
}

func (chuck *BTChuckSensor) GetBLESubscription() (subscription comms.BLESubscription) {
	subscription = comms.BLESubscription{
		Name:        Name,
		ServiceUUID: ChuckServiceUuid,
		CharacteristicUUIDs: []string{
			ChuckAcclXUuid, ChuckAcclYUuid, ChuckAcclZUuid,
			ChuckAxisXUuid, ChuckAxisYUuid,
			ChuckButtonCUuid, ChuckButtonZUuid,
		},
	}
	return
}
