#include <ArduinoBLE.h>

char TITLE[] = "CycleSense";
int BAUD_SPEED = 9600;

BLEService bikeService("1337"); // BLE LED Service
BLEFloatCharacteristic bikeRPM("1338", BLERead | BLENotify);

void setup() {
  Serial.begin(BAUD_SPEED);

  BLE.begin();
  BLE.setLocalName(TITLE);
  BLE.setDeviceName(TITLE);
  BLE.setAdvertisedService(bikeService);

  bikeService.addCharacteristic(bikeRPM);
  bikeRPM.writeValue(0);

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);
  BLE.addService(bikeService);
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

float rpms = 0;
void loop() {
  // BLE.poll(); // do we need this?
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());

    while(central.connected()) {
      Serial.print("RPMs: ");
      Serial.println(rpms);
      bikeRPM.writeValue(rpms);
      rpms++;

      delay(1000);
    }
  }
}

void connectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void disconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}