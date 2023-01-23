package manager

import (
	"cycleSenseCentral/src/sensors/bike"
	"cycleSenseCentral/src/sensors/chuck"
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
		Manager = &SensorDataManager{
			Bike:  &bike.Data{},
			Chuck: &chuck.Data{},
		}
	})

	return Manager
}
