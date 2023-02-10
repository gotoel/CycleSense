package constants

const DeviceWifiIp = "192.168.0.183"
const DeviceWifiPort = 1337
const BtMacAddr = "34:94:54:27:37:E6"

const DEVICE_CONNECTION_TYPE = WiFi

type DeviceConnectionType int64

const (
	Bluetooth   DeviceConnectionType = 0
	WiFi        DeviceConnectionType = 1
	USB         DeviceConnectionType = 2
	HybridProxy DeviceConnectionType = 3
)

const EventReset = "Reset"
