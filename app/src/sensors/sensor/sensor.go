package sensor

import (
	"cycleSenseCentral/src/ble"
	"cycleSenseCentral/src/sensors/sensor_event"
)

type Sensor struct {
	SensorCommon
	EventChannel chan<- sensor_event.SensorEvent
}

type SensorCommon interface {
	SetEventsChannel(ch chan<- sensor_event.SensorEvent)
}

func (sensor *Sensor) SetEventsChannel(ch chan<- sensor_event.SensorEvent) {
	sensor.EventChannel = ch
}

type SensorBT interface {
	Initialize()
	GetBLESubscription() ble.BLESubscription
}

type SensorWifi interface {
	InitializeHandlers()
}
