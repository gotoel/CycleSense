package chuck

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/sensors/sensor"
	"cycleSenseCentral/src/sensors/sensor_event"
	"encoding/binary"
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
	Data Data
}

func (chuck *BTChuckSensor) Initialize() {
	chuck.Data = Data{}
	chuck.setHandlers()
}

func (chuck *BTChuckSensor) setHandlers() {
	// Acceleration (X,Y,Z)
	ble.BLE.SetNotificationHandler(ChuckAcclXUuid, func(buf []byte) {
		chuck.Data.AcclX = binary.LittleEndian.Uint32(buf)
	})
	ble.BLE.SetNotificationHandler(ChuckAcclYUuid, func(buf []byte) {
		chuck.Data.AcclY = binary.LittleEndian.Uint32(buf)
	})
	ble.BLE.SetNotificationHandler(ChuckAcclZUuid, func(buf []byte) {
		chuck.Data.AcclZ = binary.LittleEndian.Uint32(buf)
	})

	// Thumb stick axis (X, Y)
	ble.BLE.SetNotificationHandler(ChuckAxisXUuid, func(buf []byte) {
		chuck.Data.AxisX = binary.LittleEndian.Uint32(buf)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
		//manager.Manager.NewEvent(sensor_event.SensorEvent{Name: Name})
		//ProcessInputs(chuck.Data)
		//println(fmt.Sprintf("(%d, %d)", chuck.AxisX, chuck.AxisY))
	})
	ble.BLE.SetNotificationHandler(ChuckAxisYUuid, func(buf []byte) {
		chuck.Data.AxisY = binary.LittleEndian.Uint32(buf)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
		//manager.Manager.NewEvent(sensor_event.SensorEvent{Name: Name})
		//ProcessInputs(chuck.Data)
		//println(fmt.Sprintf("(%d, %d)", chuck.AxisX, chuck.AxisY))
	})

	// Buttons: C, Z
	ble.BLE.SetNotificationHandler(ChuckButtonCUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.Data.ButtonC = buf[0] == 1
		}
	})
	ble.BLE.SetNotificationHandler(ChuckButtonZUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.Data.ButtonZ = buf[0] == 1
		}
	})
}

func (chuck *BTChuckSensor) GetBLESubscription() (subscription ble.BLESubscription) {
	subscription = ble.BLESubscription{
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
