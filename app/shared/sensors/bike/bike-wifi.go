package bike

import (
	"github.com/vmihailenco/msgpack/v5"
	"shared/comms"
	"shared/sensors/sensor"
	"shared/sensors/sensor_event"
)

type WifiBikeSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data Data
}

func (bike *WifiBikeSensor) InitializeHandlers() {
	comms.Handler.AddHandler(Name, func(data []byte) {
		msgpack.Unmarshal(data, &bike.Data)
		bike.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})
}
