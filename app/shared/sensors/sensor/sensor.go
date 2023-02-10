package sensor

import (
	"shared/comms"
	"shared/types"
)

type Sensor struct {
	SensorCommon
	EventChannel chan<- types.Event
}

type SensorCommon interface {
	SetEventsChannel(ch chan<- types.Event)
}

func (sensor *Sensor) SetEventsChannel(ch chan<- types.Event) {
	sensor.EventChannel = ch
}

type SensorBT interface {
	Initialize()
	GetBLESubscription() comms.BLESubscription
}

type SensorWifi interface {
	InitializeHandlers()
}
