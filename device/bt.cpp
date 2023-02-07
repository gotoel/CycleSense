#include <ArduinoBLE.h>
#include "bt.h"
#include "constants.h"
#include "bike.h"
#include "chuck.h"

// Bike service
BLEService bikeService("97bb6403-1337-4a42-8563-243ed61234c7");
BLEFloatCharacteristic bikeRPM("97bb6403-1338-4a42-8563-243ed61234c7", BLERead | BLENotify);

// Chuck service
BLEService chuckService("97bb6403-0101-4a42-8563-243ed61234c7");
BLEIntCharacteristic chuckAcclX("97bb6403-0102-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAcclY("97bb6403-0103-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAcclZ("97bb6403-0104-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisXLeft("97bb6403-0105-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisYLeft("97bb6403-0106-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyC("97bb6403-0107-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyZ("97bb6403-0108-4a42-8563-243ed61234c7", BLERead | BLENotify);

BLEIntCharacteristic chuckAxisXRight("97bb6403-0109-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisYRight("97bb6403-0110-4a42-8563-243ed61234c7", BLERead | BLENotify);

BLEBoolCharacteristic chuckKeyA("97bb6403-0111-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyB("97bb6403-0112-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyX("97bb6403-0113-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyY("97bb6403-0114-4a42-8563-243ed61234c7", BLERead | BLENotify);

BLEBoolCharacteristic chuckTriggerLeft("97bb6403-0115-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckTriggerRight("97bb6403-0116-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckTriggerZLeft("97bb6403-0117-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckTriggerZRight("97bb6403-0118-4a42-8563-243ed61234c7", BLERead | BLENotify);

BLEBoolCharacteristic chuckPadRight("97bb6403-0119-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckPadLeft("97bb6403-0120-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckPadUp("97bb6403-0121-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckPadDown("97bb6403-0122-4a42-8563-243ed61234c7", BLERead | BLENotify);

BLEBoolCharacteristic chuckKeyMinus("97bb6403-0123-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyHome("97bb6403-0124-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyPlus("97bb6403-0125-4a42-8563-243ed61234c7", BLERead | BLENotify);

void BTSetup() {
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  BLE.setLocalName(BT_TITLE);
  BLE.setDeviceName(BT_TITLE);

  // Bike service
  bikeService.addCharacteristic(bikeRPM);
  bikeRPM.writeValue(0);

  BLE.setAdvertisedService(bikeService);
  BLE.addService(bikeService);

  // Chuck service
  chuckService.addCharacteristic(chuckAcclX); 
  chuckService.addCharacteristic(chuckAcclY); 
  chuckService.addCharacteristic(chuckAcclZ);
  chuckService.addCharacteristic(chuckAxisXLeft); 
  chuckService.addCharacteristic(chuckAxisYLeft); 
  chuckService.addCharacteristic(chuckKeyC); 
  chuckService.addCharacteristic(chuckKeyZ);

  // Add Wii Classic extensions
  /*
  chuckService.addCharacteristic(chuckAxisXRight); 
  chuckService.addCharacteristic(chuckAxisYRight); 
  chuckService.addCharacteristic(chuckKeyA); 
  chuckService.addCharacteristic(chuckKeyB); 
  chuckService.addCharacteristic(chuckKeyX); 
  chuckService.addCharacteristic(chuckKeyY); 
  chuckService.addCharacteristic(chuckTriggerLeft); 
  chuckService.addCharacteristic(chuckTriggerRight); 
  chuckService.addCharacteristic(chuckTriggerZLeft); 
  chuckService.addCharacteristic(chuckTriggerZRight); 
  chuckService.addCharacteristic(chuckPadDown); 
  chuckService.addCharacteristic(chuckPadUp); 
  chuckService.addCharacteristic(chuckPadRight); 
  chuckService.addCharacteristic(chuckPadLeft); 
  chuckService.addCharacteristic(chuckKeyMinus); 
  chuckService.addCharacteristic(chuckKeyHome); 
  chuckService.addCharacteristic(chuckKeyPlus); 
*/

  chuckAcclX.writeValue(0);
  chuckAcclY.writeValue(0);
  chuckAcclZ.writeValue(0);
  chuckAxisXLeft.writeValue(0);
  chuckAxisYLeft.writeValue(0);
  chuckKeyC.writeValue(false);
  chuckKeyZ.writeValue(false);
  
  BLE.setAdvertisedService(chuckService);
  BLE.addService(chuckService);
  

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);

  BLE.advertise();

  Serial.print("BLE initialized, MAC address: ");
  Serial.println(BLE.address());
}

void BTProcessChuck(ChuckData lastData, ChuckData currentData) {
  // Update BLE values if they changed since last check
  // there's a writeValue and a writeValueLE for some reason...
  if(lastData.acclX != currentData.acclX) 
    chuckAcclX.writeValue(currentData.acclX);

  if(lastData.acclY != currentData.acclY)  
    chuckAcclY.writeValue(currentData.acclY);

  if(lastData.acclZ != currentData.acclZ) 
    chuckAcclZ.writeValue(currentData.acclZ);

  if(lastData.axisLeftX != currentData.axisLeftX) {
    chuckAxisXLeft.writeValue(currentData.axisLeftX);
  }

  if(lastData.axisLeftY != currentData.axisLeftY) 
    chuckAxisYLeft.writeValue(currentData.axisLeftY);

  if(lastData.buttonC != currentData.buttonC) 
    chuckKeyC.writeValue(currentData.buttonC);

  if(lastData.buttonZ != currentData.buttonZ) 
    chuckKeyZ.writeValue(currentData.buttonZ);


  // Wii Classic controller extensions
  if(lastData.axisRightX != currentData.axisRightX) 
    chuckAxisXRight.writeValue(currentData.axisRightX);

  if(lastData.axisRightY != currentData.axisRightY) 
    chuckAxisYRight.writeValue(currentData.axisRightY);

  // Buttons
  if(lastData.buttonA != currentData.buttonA) 
    chuckKeyA.writeValue(currentData.buttonA);
  if(lastData.buttonB != currentData.buttonB) 
    chuckKeyB.writeValue(currentData.buttonB);
  if(lastData.buttonX != currentData.buttonX) 
    chuckKeyX.writeValue(currentData.buttonX);
  if(lastData.buttonY != currentData.buttonY) 
    chuckKeyY.writeValue(currentData.buttonY);

  // Triggers
  if(lastData.triggerLeft != currentData.triggerLeft) 
    chuckTriggerLeft.writeValue(currentData.triggerLeft);
  if(lastData.triggerRight != currentData.triggerRight) 
    chuckTriggerRight.writeValue(currentData.triggerRight);
  if(lastData.triggerZLeft != currentData.triggerZLeft) 
    chuckTriggerZLeft.writeValue(currentData.triggerZLeft);
  if(lastData.triggerZRight != currentData.triggerZRight) 
    chuckTriggerZRight.writeValue(currentData.triggerZRight);


  // DPad
  if(lastData.padDown != currentData.padDown) 
    chuckPadDown.writeValue(currentData.padDown);
  if(lastData.padUp != currentData.padUp) 
    chuckPadUp.writeValue(currentData.padUp);
  if(lastData.padRight != currentData.padRight) 
    chuckPadRight.writeValue(currentData.padRight);
  if(lastData.padLeft != currentData.padLeft) 
    chuckPadLeft.writeValue(currentData.padLeft);

  // Menu/select/start buttons
  if(lastData.buttonMinus != currentData.buttonMinus) 
    chuckKeyMinus.writeValue(currentData.buttonMinus);
  if(lastData.buttonHome != currentData.buttonHome) 
    chuckKeyHome.writeValue(currentData.buttonHome);
  if(lastData.buttonPlus != currentData.buttonPlus) 
    chuckKeyPlus.writeValue(currentData.buttonPlus);
}

void BTProcessBike(BikeData bike) {
  bikeRPM.writeValue(bike.rpm);
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