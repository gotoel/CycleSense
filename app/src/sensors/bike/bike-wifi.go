package bike

import (
	"cycleSenseCentral/src/sensors/sensor"
	"cycleSenseCentral/src/sensors/sensor_event"
	"cycleSenseCentral/src/wifi"
	"encoding/json"
)

type WifiBikeSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data Data
}

func (bike *WifiBikeSensor) InitializeHandlers() {
	wifi.Handler.RegisterHandler(Name, func(data []byte) {
		json.Unmarshal(data, &bike.Data)
		bike.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
		//manager.Manager.NewEvent(sensor_event.SensorEvent{Name: Name})
		//ProcessInputs(bike.Data)
	})
}
