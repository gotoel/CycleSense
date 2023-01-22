#include <ArduinoBLE.h>
#include "chuck.h"
#include "bike.h"
#include "constants.h"
#include "bt.h"

Chuck chuck;
Bike bike;

void setup() {
  Serial.begin(BAUD_SPEED);

  switch(CONNECTION_TYPE) {
    case BLUETOOTH:
      BTSetup();
      break;
  }

  chuck.initialize();
  bike.initialize();

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
