#include <ArduinoBLE.h>
#include <WiiChuck.h>
#include "chuck.h"

Accessory nunchuck;

BLEService chuckService("97bb6403-0101-4a42-8563-243ed61234c7");
BLEIntCharacteristic chuckAcclX("97bb6403-0102-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAcclY("97bb6403-0103-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAcclZ("97bb6403-0104-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisX("97bb6403-0105-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisY("97bb6403-0106-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyC("97bb6403-0107-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyZ("97bb6403-0108-4a42-8563-243ed61234c7", BLERead | BLENotify);



void Chuck::initialize() {
	nunchuck.begin();
	if (nunchuck.type == Unknown) {
		nunchuck.type = NUNCHUCK;
	}

  chuckService.addCharacteristic(chuckAcclX); 
  chuckService.addCharacteristic(chuckAcclY); 
  chuckService.addCharacteristic(chuckAcclZ);
  chuckService.addCharacteristic(chuckAxisX); 
  chuckService.addCharacteristic(chuckAxisY); 
  chuckService.addCharacteristic(chuckKeyC); 
  chuckService.addCharacteristic(chuckKeyZ); 

  chuckAcclX.writeValue(0);
  chuckAcclY.writeValue(0);
  chuckAcclZ.writeValue(0);
  chuckAxisX.writeValue(0);
  chuckAxisY.writeValue(0);
  chuckKeyC.writeValue(false);
  chuckKeyZ.writeValue(false);
  
  BLE.setAdvertisedService(chuckService);
  BLE.addService(chuckService);
}

void Chuck::process() {
  this->lastData = this->currentData;
  this->currentData = this->collectData();

  // Update BLE values if they changed since last check
  // there's a writeValue and a writeValueLE for some reason...
  if(this->lastData.acclX != this->currentData.acclX) 
    chuckAcclX.writeValue(nunchuck.getAccelX());

  if(this->lastData.acclY != this->currentData.acclY)  
    chuckAcclY.writeValue(nunchuck.getAccelY());

  if(this->lastData.acclZ != this->currentData.acclZ) 
    chuckAcclZ.writeValue(nunchuck.getAccelZ());

  if(this->lastData.axisX != this->currentData.axisX) 
    chuckAxisX.writeValue(nunchuck.getJoyX());

  if(this->lastData.axisY != this->currentData.axisY) 
    chuckAxisY.writeValue(nunchuck.getJoyY());

  if(this->lastData.buttonC != this->currentData.axisY) 
    chuckKeyC.writeValue(nunchuck.getButtonC());

  if(this->lastData.buttonZ != this->currentData.buttonC) 
    chuckKeyZ.writeValue(nunchuck.getButtonZ());

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