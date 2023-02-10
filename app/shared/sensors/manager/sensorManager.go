package manager

import (
	"shared/sensors/bike"
	"shared/sensors/chuck"
	"shared/types"
	"sync"
)

var once sync.Once

type SensorDataManager struct {
	Bike  *bike.Data
	Chuck *chuck.Data
}

var (
	Manager *SensorDataManager
)

func NewSensorDataManager(eventChannel chan<- types.Event) *SensorDataManager {
	once.Do(func() {
		Manager = &SensorDataManager{}
	})

	return Manager
}

func (sensorDataManager *SensorDataManager) ResetSensorData() {
	sensorDataManager.Bike = &bike.Data{}
	sensorDataManager.Chuck = &chuck.Data{}
}
