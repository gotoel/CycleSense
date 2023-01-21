package sensors

import (
	"cycleSenseCentral/src/ble"
)

const (
	ChuckServiceName = "Wii Nunchuk"

	ChuckServiceUuid = "97bb6403-0101-4a42-8563-243ed61234c7"
	ChuckAxisXUuid   = "97bb6403-0102-4a42-8563-243ed61234c7"
	ChuckAxisYUuid   = "97bb6403-0103-4a42-8563-243ed61234c7"
	ChuckKeyCUuid    = "97bb6403-0104-4a42-8563-243ed61234c7"
	ChuckKeyZUuid    = "97bb6403-0105-4a42-8563-243ed61234c7"
)

type ChuckSensor struct {
	Sensor
}

func (chuck *ChuckSensor) initialize() {

}

func (chuck *ChuckSensor) GetBLESubscription() (subscription ble.BLESubscription) {
	subscription = ble.BLESubscription{
		Name:        ChuckServiceName,
		ServiceUUID: ChuckServiceUuid,
		CharacteristicUUIDs: []string{
			ChuckAxisXUuid, ChuckAxisYUuid,
			ChuckKeyCUuid, ChuckKeyZUuid,
		},
	}
	return
}
