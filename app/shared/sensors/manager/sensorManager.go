package manager

import (
	"shared/sensors/bike"
	"shared/sensors/chuck"
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

func NewSensorDataManager() *SensorDataManager {
	once.Do(func() {
		Manager = &SensorDataManager{}
	})

	return Manager
}

func (sensorDataManager *SensorDataManager) ResetSensorData() {
	sensorDataManager.Bike.Reset()
	sensorDataManager.Chuck.Reset()
}
