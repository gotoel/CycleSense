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
BLEIntCharacteristic chuckAxisX("97bb6403-0105-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisY("97bb6403-0106-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyC("97bb6403-0107-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyZ("97bb6403-0108-4a42-8563-243ed61234c7", BLERead | BLENotify);

void BTSetup() {
  BLE.begin();
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

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);

  BLE.advertise();
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

  if(lastData.axisX != currentData.axisX) 
    chuckAxisX.writeValue(currentData.axisX);

  if(lastData.axisY != currentData.axisY) 
    chuckAxisY.writeValue(currentData.axisY);

  if(lastData.buttonC != currentData.axisY) 
    chuckKeyC.writeValue(currentData.axisY);

  if(lastData.buttonZ != currentData.buttonC) 
    chuckKeyZ.writeValue(currentData.buttonC);
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