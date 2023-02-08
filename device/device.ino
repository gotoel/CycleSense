#include <ArduinoBLE.h>
#include "chuck.h"
#include "bike.h"
#include "constants.h"
#include "bt.h"
#include "wifi.h"
#include <WiiChuck.h>

Chuck chuck;
Bike bike;
Accessory nunchuck1;

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
  /*
	Serial.println("-------------------------------------------");
	nunchuck1.readData();    // Read inputs and update maps
	//nunchuck1.printInputs(); // Print all inputs
  nunchuck1.printInputsClassic();
*/

  switch(CONNECTION_TYPE) {
  case BLUETOOTH:
    // Handles bluetooth events
    BLE.poll(); 
    break;   
  case WIFI:
    ProcessWifi();
    break;
  }

  chuck.process();
  bike.process();

  // DELAY FOR STABILITY, REMOVE FOR PRODUCTION
  delay(10);
}
