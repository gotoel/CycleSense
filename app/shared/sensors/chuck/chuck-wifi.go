package chuck

import (
	"github.com/vmihailenco/msgpack/v5"
	"shared/comms"
	"shared/sensors/sensor"
	"shared/sensors/sensor_event"
)

type WifiChuckSensor struct {
	sensor.Sensor
	sensor.SensorWifi
	Data Data
}

func (chuck *WifiChuckSensor) InitializeHandlers() {
	comms.Handler.AddHandler("ChuckInfo", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Info)
		//chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})

	comms.Handler.AddHandler("ChuckAccelerometer", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Accelerometer)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})

	comms.Handler.AddHandler("ChuckSticks", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Sticks)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})

	comms.Handler.AddHandler("ChuckButtons", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Buttons)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})

	comms.Handler.AddHandler("ChuckTriggers", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Triggers)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})

	comms.Handler.AddHandler("ChuckDpad", func(data []byte) {
		msgpack.Unmarshal(data, &chuck.Data.Dpad)
		chuck.Sensor.EventChannel <- sensor_event.SensorEvent{Name: Name}
	})
}
