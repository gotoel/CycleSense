package main

import (
	"encoding/binary"
	"encoding/json"
	"shared/comms"
	"shared/sensors/chuck"
)

type BTChuckSensorProxy struct {
	chuck.BTChuckSensor
}

type BTChuckDataProxy struct {
	dataType string `json:"type"`
	chuck.Data
}

func (chp *BTChuckSensorProxy) Initialize() {
	chp.Data = chuck.Data{}
	chp.setHandlers()
}

func (chp *BTChuckSensorProxy) setHandlers() {
	// Acceleration (X,Y,Z)
	// unused... so not implemented in proxy

	// Thumb stick axis (X, Y)
	comms.BLE.SetNotificationHandler(chuck.ChuckAxisXUuid, func(buf []byte) {
		chp.Data.AxisX = binary.LittleEndian.Uint32(buf)
		chp.proxyData()
	})
	comms.BLE.SetNotificationHandler(chuck.ChuckAxisYUuid, func(buf []byte) {
		chp.Data.AxisY = binary.LittleEndian.Uint32(buf)
		chp.proxyData()
	})

	// Buttons: C, Z
	comms.BLE.SetNotificationHandler(chuck.ChuckButtonCUuid, func(buf []byte) {
		if len(buf) > 0 {
			chp.Data.ButtonC = buf[0] == 1
		}
		chp.proxyData()
	})
	comms.BLE.SetNotificationHandler(chuck.ChuckButtonZUuid, func(buf []byte) {
		if len(buf) > 0 {
			chp.Data.ButtonZ = buf[0] == 1
		}
		chp.proxyData()
	})
}

func (chp *BTChuckSensorProxy) proxyData() {
	chdp := &BTChuckDataProxy{
		dataType: chuck.Name,
		Data:     chp.Data,
	}

	data, _ := json.Marshal(chdp)
	Handler.SendData(data)
}

func (chp *BTChuckSensorProxy) GetBLESubscription() (subscription comms.BLESubscription) {
	subscription = comms.BLESubscription{
		Name:        chuck.Name,
		ServiceUUID: chuck.ChuckServiceUuid,
		CharacteristicUUIDs: []string{
			chuck.ChuckAcclXUuid, chuck.ChuckAcclYUuid, chuck.ChuckAcclZUuid,
			chuck.ChuckAxisXUuid, chuck.ChuckAxisYUuid,
			chuck.ChuckButtonCUuid, chuck.ChuckButtonZUuid,
		},
	}
	return
}
