package src

const CS_DEVICE_ADDR = "34:94:54:27:6A:BE"

const DEVICE_CONNECTION_TYPE = Bluetooth

type DeviceConnectionType int64

const (
	Bluetooth DeviceConnectionType = 0
	WiFi                           = 1
	USB                            = 2
)
