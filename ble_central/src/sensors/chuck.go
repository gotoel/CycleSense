package sensors

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/input"
	"encoding/binary"
)

const (
	ChuckServiceName = "Wii Nunchuk"

	ChuckServiceUuid = "97bb6403-0101-4a42-8563-243ed61234c7"
	ChuckAcclXUuid   = "97bb6403-0102-4a42-8563-243ed61234c7"
	ChuckAcclYUuid   = "97bb6403-0103-4a42-8563-243ed61234c7"
	ChuckAcclZUuid   = "97bb6403-0104-4a42-8563-243ed61234c7"
	ChuckAxisXUuid   = "97bb6403-0105-4a42-8563-243ed61234c7"
	ChuckAxisYUuid   = "97bb6403-0106-4a42-8563-243ed61234c7"
	ChuckButtonCUuid = "97bb6403-0107-4a42-8563-243ed61234c7"
	ChuckButtonZUuid = "97bb6403-0108-4a42-8563-243ed61234c7"
)

type ChuckSensor struct {
	Sensor

	AcclX   uint32
	AcclY   uint32
	AcclZ   uint32
	AxisX   uint32
	AxisY   uint32
	ButtonC bool
	ButtonZ bool
}

func (chuck *ChuckSensor) initialize() {
	chuck.setHandlers()
}

func (chuck *ChuckSensor) setHandlers() {
	// Acceleration (X,Y,Z)
	ble.BLE.SetNotificationHandler(ChuckAcclXUuid, func(buf []byte) {
		chuck.AcclX = binary.LittleEndian.Uint32(buf)
	})
	ble.BLE.SetNotificationHandler(ChuckAcclYUuid, func(buf []byte) {
		chuck.AcclY = binary.LittleEndian.Uint32(buf)
	})
	ble.BLE.SetNotificationHandler(ChuckAcclZUuid, func(buf []byte) {
		chuck.AcclZ = binary.LittleEndian.Uint32(buf)
	})

	// Thumb stick axis (X, Y)
	ble.BLE.SetNotificationHandler(ChuckAxisXUuid, func(buf []byte) {
		chuck.AxisX = binary.LittleEndian.Uint32(buf)
		chuck.updateSenseInput()
		//println(fmt.Sprintf("(%d, %d)", chuck.AxisX, chuck.AxisY))
	})
	ble.BLE.SetNotificationHandler(ChuckAxisYUuid, func(buf []byte) {
		chuck.AxisY = binary.LittleEndian.Uint32(buf)
		chuck.updateSenseInput()
		//println(fmt.Sprintf("(%d, %d)", chuck.AxisX, chuck.AxisY))
	})

	// Buttons: C, Z
	ble.BLE.SetNotificationHandler(ChuckButtonCUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.ButtonC = buf[0] == 1
		}
	})
	ble.BLE.SetNotificationHandler(ChuckButtonZUuid, func(buf []byte) {
		if len(buf) > 0 {
			chuck.ButtonZ = buf[0] == 1
		}
	})
}

func (chuck *ChuckSensor) updateSenseInput() {
	senseAxisX := (float32(chuck.AxisX) * (2) / 255) - 1
	senseAxisY := (float32(chuck.AxisY) * (2) / 255) - 1

	//println(fmt.Sprintf("(%0.2f, %0.2f)", senseAxisX, senseAxisY))
	input.Input.SetAxis(senseAxisX, senseAxisY)
}

func (chuck *ChuckSensor) GetBLESubscription() (subscription ble.BLESubscription) {
	subscription = ble.BLESubscription{
		Name:        ChuckServiceName,
		ServiceUUID: ChuckServiceUuid,
		CharacteristicUUIDs: []string{
			ChuckAcclXUuid, ChuckAcclYUuid, ChuckAcclZUuid,
			ChuckAxisXUuid, ChuckAxisYUuid,
			ChuckButtonCUuid, ChuckButtonZUuid,
		},
	}
	return
}
