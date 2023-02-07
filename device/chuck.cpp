#include <ArduinoBLE.h>
#include <NintendoExtensionCtrl.h>
#include "chuck.h"
#include "bt.h"
#include "wifi.h"
#include "constants.h"

ExtensionPort port;  // Port for communicating with extension controllers
ExtensionType conType;
int controllerTypeId;

ClassicController::Shared classic(port);
Nunchuk::Shared nchuk(port);

void Chuck::initialize() {
  this->lastData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false};
  this->currentData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false};

	port.begin();
  port.connect();

  // Identify controller
  conType = port.getControllerType();
	switch (conType) {
  case(ExtensionType::NoController):
      Serial.println("No controller detected");
      break;
    case(ExtensionType::UnknownController):
      Serial.println("Unknown controller connected");
      break;
    case(ExtensionType::Nunchuk):
      Serial.println("Nunchuk connected!");
      this->currentData.controllerType = 0;
      break;
    case(ExtensionType::ClassicController):
      Serial.println("Classic Controller connected!");
      this->currentData.controllerType = 1;
      break;
    default: break;
  }
}

void Chuck::process() {
  this->lastData = this->currentData;
  this->currentData = this->collectData();

  this->sendData();
  // set to true for debugging
  if(false) {
    this->debugPrint();
    delay(100);
  }
}

ChuckData Chuck::collectData() {
  ChuckData data;
  boolean success = port.update();  // Get new data from the controller

  if (success == true) {  // We've got data!
    switch (conType) {
      case(ExtensionType::Nunchuk):
      {   
        return {0, nchuk.accelX(), nchuk.accelY(), nchuk.accelZ(), nchuk.joyX(), nchuk.joyY(), nchuk.buttonC(), nchuk.buttonZ(), 
        0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
      }
      break;
      case(ExtensionType::ClassicController):   
      {
        /*
        data.controllerType = 1;

        data.acclX = 0;
        data.acclY = 0;
        data.acclZ = 0;

        data.axisLeftX = classic.leftJoyX();
        data.axisLeftX = classic.leftJoyY();
        data.axisRightX = classic.rightJoyX();
        data.axisRightY = classic.rightJoyY();

        data.buttonA = classic.buttonA();
        data.buttonB = classic.buttonB();
        data.buttonX = classic.buttonX();
        data.buttonY = classic.buttonY();

        data.buttonMinus = classic.buttonMinus();
        data.buttonHome = classic.buttonHome();
        data.buttonPlus = classic.buttonPlus();

        data.triggerLeft = classic.triggerL();
        data.triggerRight = classic.triggerR();
        data.triggerZLeft = classic.buttonZL();
        data.triggerZRight = classic.buttonZR();

        data.padDown = classic.dpadDown();
        data.padUp = classic.dpadUp();
        data.padRight = classic.dpadRight();
        data.padLeft = classic.dpadLeft();
        */
   
        return {1, 0, 0, 0, classic.leftJoyX(), classic.leftJoyY(), classic.rightJoyX(), classic.rightJoyY(), 
        false, false, 
        classic.buttonA(), classic.buttonB(), classic.buttonX(), classic.buttonY(), 
        classic.buttonZL(), classic.triggerL(), classic.triggerR(), classic.buttonZR(), 
        classic.buttonMinus(), classic.buttonHome(), classic.buttonPlus(), 
        classic.dpadLeft(), classic.dpadUp(), classic.dpadRight(), classic.dpadDown()};
      }
      break;
      default:
      break;
    }
  }
  else {  
    // bad data, return current data
    Serial.println("BAD");
    return this->currentData;
  } 
}

ChuckData Chuck::getCurrentData() {
  return this->currentData;
}

bool Chuck::valuesChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.acclX != currentData.acclX || lastData.acclY != currentData.acclY ||
    lastData.acclZ != currentData.acclZ || 
    lastData.axisLeftX != currentData.axisLeftX || lastData.axisLeftY != currentData.axisLeftY || 
    lastData.buttonC != currentData.buttonC || lastData.buttonZ != currentData.buttonZ || 
    lastData.axisRightX != currentData.axisRightX || lastData.axisRightY != currentData.axisRightY ||
    lastData.buttonX != currentData.buttonX || lastData.buttonY != currentData.buttonY ||
    lastData.buttonA != currentData.buttonA || lastData.buttonB != currentData.buttonB || 
    lastData.triggerZLeft != currentData.triggerZLeft || lastData.triggerZRight != currentData.triggerZRight ||
    lastData.triggerLeft != currentData.triggerLeft || lastData.triggerRight != currentData.triggerRight ||
    lastData.padUp != currentData.padUp || lastData.padDown != currentData.padDown ||
    lastData.padRight != currentData.padRight || lastData.padLeft != currentData.padLeft ||
    lastData.buttonMinus != currentData.buttonMinus || lastData.buttonHome != lastData.buttonHome || 
    lastData.buttonPlus != lastData.buttonPlus) {
      return true;
  }

  return false;
}

// Sends updated data down selected connection type
void Chuck::sendData() {
  switch(CONNECTION_TYPE) {
    case BLUETOOTH: 
    {
      BTProcessChuck(this->lastData, this->currentData);
      if(this->valuesChanged()) 
        BLE.poll();
    }
    break;
    case WIFI: 
    {
      if(this->valuesChanged()) {
        WifiSendChuckData(this->currentData);
      }
    }
    break;
  }
}

void Chuck::debugPrint() {
  Serial.print("X: "); Serial.print(this->currentData.acclX);
  Serial.print(" \tY: "); Serial.print(this->currentData.acclY); 
  Serial.print(" \tZ: ");  Serial.print(this->currentData.acclZ);

  Serial.print("\t\tLeft Joy: ("); 
  Serial.print(this->currentData.axisLeftX);
  Serial.print(", "); 
  Serial.print(this->currentData.axisLeftY);
  Serial.print(")");

  
  Serial.print("\t\tRight Joy: ("); 
  Serial.print(this->currentData.axisRightX);
  Serial.print(", "); 
  Serial.print(this->currentData.axisRightY);
  Serial.print(")");

  Serial.print("\t\tButton: "); 
  if (this->currentData.buttonZ) Serial.print(" Z "); 
  if (this->currentData.buttonC) Serial.print(" C "); 

  if (this->currentData.buttonA) Serial.print(" A "); 
  if (this->currentData.buttonB) Serial.print(" B "); 
  if (this->currentData.buttonX) Serial.print(" X "); 
  if (this->currentData.buttonY) Serial.print(" Y "); 

  Serial.println();
}
