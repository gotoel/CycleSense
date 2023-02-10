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
  if(lastData.accelerometer.acclX != currentData.accelerometer.acclX) 
    chuckAcclX.writeValue(currentData.accelerometer.acclX);

  if(lastData.accelerometer.acclY != currentData.accelerometer.acclY)  
    chuckAcclY.writeValue(currentData.accelerometer.acclY);

  if(lastData.accelerometer.acclZ != currentData.accelerometer.acclZ) 
    chuckAcclZ.writeValue(currentData.accelerometer.acclZ);

  if(lastData.sticks.axisLeftX != currentData.sticks.axisLeftX) {
    chuckAxisXLeft.writeValue(currentData.sticks.axisLeftX);
  }

  if(lastData.sticks.axisLeftY != currentData.sticks.axisLeftY) 
    chuckAxisYLeft.writeValue(currentData.sticks.axisLeftY);

  if(lastData.buttons.buttonC != currentData.buttons.buttonC) 
    chuckKeyC.writeValue(currentData.buttons.buttonC);

  if(lastData.buttons.buttonZ != currentData.buttons.buttonZ) 
    chuckKeyZ.writeValue(currentData.buttons.buttonZ);


  // Wii Classic controller extensions
  if(lastData.sticks.axisRightX != currentData.sticks.axisRightX) 
    chuckAxisXRight.writeValue(currentData.sticks.axisRightX);

  if(lastData.sticks.axisRightY != currentData.sticks.axisRightY) 
    chuckAxisYRight.writeValue(currentData.sticks.axisRightY);

  // Buttons
  if(lastData.buttons.buttonA != currentData.buttons.buttonA) 
    chuckKeyA.writeValue(currentData.buttons.buttonA);
  if(lastData.buttons.buttonB != currentData.buttons.buttonB) 
    chuckKeyB.writeValue(currentData.buttons.buttonB);
  if(lastData.buttons.buttonX != currentData.buttons.buttonX) 
    chuckKeyX.writeValue(currentData.buttons.buttonX);
  if(lastData.buttons.buttonY != currentData.buttons.buttonY) 
    chuckKeyY.writeValue(currentData.buttons.buttonY);

  // Triggers
  if(lastData.triggers.triggerLeft != currentData.triggers.triggerLeft) 
    chuckTriggerLeft.writeValue(currentData.triggers.triggerLeft);
  if(lastData.triggers.triggerRight != currentData.triggers.triggerRight) 
    chuckTriggerRight.writeValue(currentData.triggers.triggerRight);
  if(lastData.triggers.triggerZLeft != currentData.triggers.triggerZLeft) 
    chuckTriggerZLeft.writeValue(currentData.triggers.triggerZLeft);
  if(lastData.triggers.triggerZRight != currentData.triggers.triggerZRight) 
    chuckTriggerZRight.writeValue(currentData.triggers.triggerZRight);


  // DPad
  if(lastData.dpad.padDown != currentData.dpad.padDown) 
    chuckPadDown.writeValue(currentData.dpad.padDown);
  if(lastData.dpad.padUp != currentData.dpad.padUp) 
    chuckPadUp.writeValue(currentData.dpad.padUp);
  if(lastData.dpad.padRight != currentData.dpad.padRight) 
    chuckPadRight.writeValue(currentData.dpad.padRight);
  if(lastData.dpad.padLeft != currentData.dpad.padLeft) 
    chuckPadLeft.writeValue(currentData.dpad.padLeft);

  // Menu/select/start buttons
  if(lastData.buttons.buttonMinus != currentData.buttons.buttonMinus) 
    chuckKeyMinus.writeValue(currentData.buttons.buttonMinus);
  if(lastData.buttons.buttonHome != currentData.buttons.buttonHome) 
    chuckKeyHome.writeValue(currentData.buttons.buttonHome);
  if(lastData.buttons.buttonPlus != currentData.buttons.buttonPlus) 
    chuckKeyPlus.writeValue(currentData.buttons.buttonPlus);
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