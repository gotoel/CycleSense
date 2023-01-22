#include <ArduinoBLE.h>
#include "chuck.h"
#include "bike.h"

char TITLE[] = "CycleSense";
const int BAUD_SPEED = 9600;

Chuck chuck;
Bike bike;

void setup() {
  Serial.begin(BAUD_SPEED);

  BLE.begin();
  BLE.setLocalName(TITLE);
  BLE.setDeviceName(TITLE);

  chuck.initialize();
  bike.initialize();

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);

  BLE.advertise();

  Serial.println("CycleSense active, waiting for connections...");
}

void loop() {
  // Handles bluetooth events
  BLE.poll();

  chuck.process();
  bike.process();

  // DELAY FOR STABILITY, REMOVE FOR PRODUCTION
  //delay(100);
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