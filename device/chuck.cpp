#include <ArduinoBLE.h>
#include <WiiChuck.h>
#include "chuck.h"
#include "bt.h"
#include "wifi.h"
#include "constants.h"

Accessory nunchuck;

void Chuck::initialize() {
	nunchuck.begin();
	if (nunchuck.type == Unknown) {
		nunchuck.type = NUNCHUCK;
	}
}

void Chuck::process() {
  this->lastData = this->currentData;
  this->currentData = this->collectData();

  this->sendData();

  // set to true for debugging
  if(false) {
    this->debugPrint();
    Serial.println();
    delay(200);
  }
}

ChuckData Chuck::collectData() {
  nunchuck.readData();    // Read inputs and update maps

  return {nunchuck.getAccelX(), nunchuck.getAccelY(), nunchuck.getAccelZ(), 
          nunchuck.getJoyX(), nunchuck.getJoyY(),
          nunchuck.getButtonC(), nunchuck.getButtonZ()};
}

// Sends updated data down selected connection type
void Chuck::sendData() {
  switch(CONNECTION_TYPE) {
    case BLUETOOTH:
      BTProcessChuck(this->lastData, this->currentData);
    case WIFI:
      WifiProcessChuck(this->lastData, this->currentData);
      break;
  }
}

void Chuck::debugPrint() {
  Serial.print("X: "); Serial.print(nunchuck.getAccelX());
  Serial.print(" \tY: "); Serial.print(nunchuck.getAccelY()); 
  Serial.print(" \tZ: "); Serial.println(nunchuck.getAccelZ()); 

  Serial.print("Joy: ("); 
  Serial.print(nunchuck.getJoyX());
  Serial.print(", "); 
  Serial.print(nunchuck.getJoyY());
  Serial.println(")");

  Serial.print("Button: "); 
  if (nunchuck.getButtonZ()) Serial.print(" Z "); 
  if (nunchuck.getButtonC()) Serial.print(" C "); 
}