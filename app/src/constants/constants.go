package constants

const DeviceWifiIp = "192.168.0.226"
const BtMacAddr = "34:94:54:27:6A:BE"

const DEVICE_CONNECTION_TYPE = WiFi

type DeviceConnectionType int64

const (
	Bluetooth DeviceConnectionType = 0
	WiFi      DeviceConnectionType = 1
	USB       DeviceConnectionType = 2
)
