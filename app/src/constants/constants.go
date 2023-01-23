package constants

const WIFI_CONNECT_MSG = "CYCLESENSE|CONNECT"

const DEVICE_WIFI_IP = "192.168.0.225"
const BT_MAC_ADDR = "34:94:54:27:6A:BE"

const DEVICE_CONNECTION_TYPE = WiFi

type DeviceConnectionType int64

const (
	Bluetooth DeviceConnectionType = 0
	WiFi      DeviceConnectionType = 1
	USB       DeviceConnectionType = 2
)
