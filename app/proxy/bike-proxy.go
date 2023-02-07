package main

import (
	"encoding/binary"
	"encoding/json"
	"math"
	"shared/comms"
	"shared/sensors/bike"
)

type BTBikeSensorProxy struct {
	bike.BTBikeSensor
}

type BTBikeDataProxy struct {
	dataType string `json:"type"`
	bike.Data
}

func (bp *BTBikeSensorProxy) Initialize() {
	bp.Data = bike.Data{}
	bp.setHandlers()
}

func (bp *BTBikeSensorProxy) setHandlers() {
	// Set RPM handler
	comms.BLE.SetNotificationHandler(bike.RpmUuid, func(buf []byte) {
		bits := binary.LittleEndian.Uint32(buf)
		float := math.Float32frombits(bits)
		bp.Data.RPM = float

		// Proxy over Wifi
		bdp := &BTBikeDataProxy{
			dataType: bike.Name,
			Data:     bp.Data,
		}

		data, _ := json.Marshal(bdp)
		Handler.SendData(data)
	})
}

func (bp *BTBikeSensorProxy) GetBLESubscription() (subscription comms.BLESubscription) {
	subscription = comms.BLESubscription{
		Name:        bike.Name,
		ServiceUUID: bike.BikeServiceUuid,
		CharacteristicUUIDs: []string{
			bike.RpmUuid,
		},
	}
	return
}
