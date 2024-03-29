package chuck

import (
	"encoding/json"
	"shared/comms"
	"shared/sensors/sensor"
	"shared/types"
)

type WifiChuckSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data *Data
}

func (chuck *WifiChuckSensor) InitializeHandlers() {
	comms.Handler.AddHandler("ChuckInfo", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Info)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	comms.Handler.AddHandler("ChuckAccelerometer", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Accelerometer)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	comms.Handler.AddHandler("ChuckSticks", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Sticks)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	comms.Handler.AddHandler("ChuckButtons", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Buttons)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	comms.Handler.AddHandler("ChuckTriggers", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Triggers)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})

	comms.Handler.AddHandler("ChuckDpad", func(data []byte) {
		json.Unmarshal(data, &chuck.Data.Dpad)
		chuck.Sensor.EventChannel <- types.Event{Name: Name}
	})
}
