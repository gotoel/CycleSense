package sensor

import "cycleSenseCentral/src/ble"

type SensorBT interface {
	Initialize()
	GetBLESubscription() ble.BLESubscription
}
