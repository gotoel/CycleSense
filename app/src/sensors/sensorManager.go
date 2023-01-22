package sensors

import (
	"cycleSenseCentral/src/ble"
	"sync"
)

var once sync.Once

type Sensor interface {
	GetBLESubscription() ble.BLESubscription
	initialize()
}

type SensorManager struct {
	Bike  *BikeSensor
	Chuck *ChuckSensor
}

var (
	Manager *SensorManager
)

func NewSensorManager() *SensorManager {
	once.Do(func() {
		Manager = &SensorManager{
			Bike:  &BikeSensor{},
			Chuck: &ChuckSensor{},
		}
	})

	return Manager
}

func (sensors *SensorManager) InitializeSensors() {
	sensors.Bike.initialize()
	sensors.Chuck.initialize()
}
