package sensor

import (
	"shared/comms"
	"shared/sensors/sensor_event"
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
	GetBLESubscription() comms.BLESubscription
}

type SensorWifi interface {
	InitializeHandlers()
}
