package chuck

import (
	"cycleSenseCentral/src/sensors/sensor"
	"cycleSenseCentral/src/sensors/sensor_event"
	"cycleSenseCentral/src/wifi"
	"encoding/json"
)

type WifiChuckSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data Data
}

func (chuck *WifiChuckSensor) InitializeHandlers() {
	wifi.Handler.AddHandler(Name, func(data []byte) {
		json.Unmarshal(data, &chuck.Data)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})
}
