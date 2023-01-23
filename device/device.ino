#include <ArduinoBLE.h>
#include "chuck.h"
#include "bike.h"
#include "constants.h"
#include "bt.h"
#include "wifi.h"

Chuck chuck;
Bike bike;

void setup() {
  Serial.begin(BAUD_SPEED);

  switch(CONNECTION_TYPE) {
    case BLUETOOTH:
      BTSetup();
      break;
    case WIFI:
      WifiSetup();
      break;
  }

  chuck.initialize();
  bike.initialize();

  Serial.println("CycleSense active, waiting for connections...");
}

void loop() {
  switch(CONNECTION_TYPE) {
  case BLUETOOTH:
    // Handles bluetooth events
    BLE.poll();    
  case WIFI:
    ProcessWifi();
    break;
  }

  chuck.process();
  bike.process();

  // DELAY FOR STABILITY, REMOVE FOR PRODUCTION
  //delay(100);
}
