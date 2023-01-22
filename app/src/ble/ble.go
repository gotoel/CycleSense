package ble

import (
	"fmt"
	"sync"
	"time"
	"tinygo.org/x/bluetooth"
)

var once sync.Once

type BLESubscription struct {
	Name                string
	ServiceUUID         string
	CharacteristicUUIDs []string
}

type Handler struct {
	peripheralMAC string

	device *bluetooth.Device

	subscriptions []BLESubscription
	serviceMap    map[string]bluetooth.DeviceService
	charMap       map[string]bluetooth.DeviceCharacteristic
}

var (
	BLE     *Handler
	adapter = bluetooth.DefaultAdapter
)

func NewBLEHandler(peripheralMAC string, subscriptions []BLESubscription) *Handler {
	once.Do(func() {
		BLE = &Handler{
			peripheralMAC: peripheralMAC,
			subscriptions: subscriptions,

			serviceMap: make(map[string]bluetooth.DeviceService),
			charMap:    make(map[string]bluetooth.DeviceCharacteristic),
		}
	})

	return BLE
}

func (ble Handler) InitializeBluetooth() {
	// Enable BLE interface.
	must("enable BLE stack", adapter.Enable())

	ch := make(chan bluetooth.ScanResult, 1)

	// Start scanning.
	println("scanning...")
	err := adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
		if result.Address.String() == ble.peripheralMAC {
			now := time.Now()
			println(now.Format(time.Stamp), " - found device:", result.Address.String(), result.RSSI, result.LocalName())
			adapter.StopScan()
			ch <- result
		}
	})

	select {
	case result := <-ch:
		ble.device, err = adapter.Connect(result.Address, bluetooth.ConnectionParams{})
		if err != nil {
			println(err.Error())
			return
		}

		println("connected to ", result.Address.String())
	}

	ble.discoverSubscriptions(ble.subscriptions)
}

func (ble Handler) strToUuid(uuidStr string) (uuid bluetooth.UUID, err error) {
	uuid, err = bluetooth.ParseUUID(uuidStr)
	return
}

func (ble Handler) strToUuidSlice(uuidStr []string) (uuids []bluetooth.UUID) {
	for _, str := range uuidStr {
		uuid, err := ble.strToUuid(str)
		if err != nil {
			fmt.Println(fmt.Sprintf("Error retrieving "+
				"characteristic (%s) from device (%s)", str, ble.peripheralMAC))
		}
		uuids = append(uuids, uuid)
	}
	return
}

func (ble Handler) discoverSubscriptions(subscriptions []BLESubscription) {
	// Pull out service IDs and discover services first
	var serviceIds []string
	for _, subscription := range subscriptions {
		serviceIds = append(serviceIds, subscription.ServiceUUID)
	}

	println("discovering services")
	ble.discoverServices(serviceIds)

	for _, subscription := range subscriptions {
		if _, ok := ble.serviceMap[subscription.ServiceUUID]; ok {
			println("discovering characteristics for service: " + subscription.Name)
			ble.discoverCharacteristics(subscription.ServiceUUID, subscription.CharacteristicUUIDs)
		}
	}
}

func (ble Handler) discoverServices(serviceUuids []string) {
	services, err := ble.device.DiscoverServices(ble.strToUuidSlice(serviceUuids))
	must("discover services", err)

	for _, service := range services {
		ble.serviceMap[service.UUID().String()] = service
	}

	println(fmt.Sprintf("collected %d/%d requested services.", len(ble.serviceMap), len(serviceUuids)))
}

func (ble Handler) discoverCharacteristics(serviceUuid string, characteristicUuids []string) {
	if service, ok := ble.serviceMap[serviceUuid]; ok {
		chars, err := service.DiscoverCharacteristics(ble.strToUuidSlice(characteristicUuids))
		if err != nil {
			println(err)
		}

		for _, char := range chars {
			ble.charMap[char.UUID().String()] = char
		}

		println(fmt.Sprintf("collected %d/%d requested characteristics.", len(chars), len(characteristicUuids)))

	} else {
		fmt.Printf("BLE Service not found in service map: %d\n", serviceUuid)
	}
}

func (ble Handler) SetNotificationHandler(characteristicUuid string, callback func(buf []byte)) {
	ble.charMap[characteristicUuid].EnableNotifications(callback)
}

func must(action string, err error) {
	if err != nil {
		panic("failed to " + action + ": " + err.Error())
	}
}
