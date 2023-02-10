package bike

import (
	"encoding/json"
	"shared/comms"
	"shared/sensors/sensor"
	"shared/types"
)

type WifiBikeSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data Data
}

func (bike *WifiBikeSensor) InitializeHandlers() {
	comms.Handler.AddHandler(Name, func(data []byte) {
		json.Unmarshal(data, &bike.Data)
		bike.Sensor.EventChannel <- types.Event{Name: Name}
	})
}
