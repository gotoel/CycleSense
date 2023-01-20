#include <ArduinoBLE.h>
#include "chuck.h"

BLEService chuckService("97bb6403-0101-4a42-8563-243ed61234c7");
BLEIntCharacteristic chuckAxisX("97bb6403-0102-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEIntCharacteristic chuckAxisY("97bb6403-0103-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyC("97bb6403-0104-4a42-8563-243ed61234c7", BLERead | BLENotify);
BLEBoolCharacteristic chuckKeyZ("97bb6403-0105-4a42-8563-243ed61234c7", BLERead | BLENotify);

void Chuck::initialize() {
  chuckService.addCharacteristic(chuckAxisX); 
  chuckService.addCharacteristic(chuckAxisY); 
  chuckService.addCharacteristic(chuckKeyC); 
  chuckService.addCharacteristic(chuckKeyZ); 

  chuckAxisX.writeValue(0);
  chuckAxisY.writeValue(0);
  chuckKeyC.writeValue(false);
  chuckKeyZ.writeValue(false);
  
  BLE.setAdvertisedService(chuckService);
  BLE.addService(chuckService);
}

void Chuck::process() {

}