#include <ArduinoBLE.h>
#include <NintendoExtensionCtrl.h>
#include "chuck.h"
#include "bt.h"
#include "constants.h"
#include "wifi.h"

ExtensionPort port;  // Port for communicating with extension controllers
ExtensionType conType;
int controllerTypeId;

ClassicController::Shared classic(port);
Nunchuk::Shared nchuk(port);

void Chuck::initialize() {
  //this->lastData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false};
  //this->currentData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,false,false,false,false,false,false,false};

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
      this->currentData.info.controllerType = 0;
      break;
    case(ExtensionType::ClassicController):
      Serial.println("Classic Controller connected!");
      this->currentData.info.controllerType = 1;
      break;
    default: break;
  }
}

void Chuck::process() {
  this->lastData = this->currentData;
  this->collectData();

  this->sendData();
  // set to true for debugging
  if(false) {
    this->debugPrint();
    delay(100);
  }
}

void Chuck::collectData() {
  boolean success = port.update();  // Get new data from the controller

  if (success == true) {  // We've got data!
    switch (conType) {
      case(ExtensionType::Nunchuk):
      {   
        this->currentData.info.controllerType = 0;
        this->currentData.accelerometer.acclX = nchuk.accelX();
        this->currentData.accelerometer.acclY = nchuk.accelY();
        this->currentData.accelerometer.acclZ = nchuk.accelZ();


        this->currentData.buttons.buttonC = nchuk.buttonC();
        this->currentData.buttons.buttonZ = nchuk.buttonZ();
      }
      break;
      case(ExtensionType::ClassicController):   
      {
        Serial.println(classic.buttonL());
        
        this->currentData.info.controllerType = 1;
        this->currentData.accelerometer.acclX = 0;
        this->currentData.accelerometer.acclY = 0;
        this->currentData.accelerometer.acclZ = 0;

        this->currentData.sticks.axisLeftX = classic.leftJoyX();
        this->currentData.sticks.axisLeftY = classic.leftJoyY();
        this->currentData.sticks.axisRightX = classic.rightJoyX();
        this->currentData.sticks.axisRightY = classic.rightJoyY();

        this->currentData.buttons.buttonA = classic.buttonA();
        this->currentData.buttons.buttonB = classic.buttonB();
        this->currentData.buttons.buttonX = classic.buttonX();
        this->currentData.buttons.buttonY = classic.buttonY();
        this->currentData.buttons.buttonMinus = classic.buttonMinus();
        this->currentData.buttons.buttonHome = classic.buttonHome();
        this->currentData.buttons.buttonPlus = classic.buttonPlus();

        this->currentData.triggers.triggerLeft = classic.buttonL();
        this->currentData.triggers.triggerRight = classic.buttonR();
        this->currentData.triggers.triggerZLeft = classic.buttonZL();
        this->currentData.triggers.triggerZRight = classic.buttonZR();

        this->currentData.dpad.padUp = classic.dpadUp();
        this->currentData.dpad.padDown = classic.dpadDown();
        this->currentData.dpad.padLeft = classic.dpadLeft();
        this->currentData.dpad.padRight = classic.dpadRight();
      }
      break;
      default:
      break;
    }
  }
  else {  
    // bad data, return current data
    Serial.println("BAD");
  } 
}

ChuckData Chuck::getCurrentData() {
  return this->currentData;
}

bool Chuck::infoChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.info.controllerType != currentData.info.controllerType) {
    return true;
  }
  return false;
}

bool Chuck::acclChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.accelerometer.acclX != currentData.accelerometer.acclX || lastData.accelerometer.acclY != currentData.accelerometer.acclY ||
    lastData.accelerometer.acclZ != currentData.accelerometer.acclZ) {
      return true;
    }
  return false;
}

bool Chuck::axisChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.sticks.axisLeftX != currentData.sticks.axisLeftX || lastData.sticks.axisLeftY != currentData.sticks.axisLeftY || 
  lastData.sticks.axisRightX != currentData.sticks.axisRightX || lastData.sticks.axisRightY != currentData.sticks.axisRightY) {
    return true;
  }
  return false;
}

bool Chuck::triggerChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.triggers.triggerZLeft != currentData.triggers.triggerZLeft || lastData.triggers.triggerZRight != currentData.triggers.triggerZRight ||
    lastData.triggers.triggerLeft != currentData.triggers.triggerLeft || lastData.triggers.triggerRight != currentData.triggers.triggerRight) {
      return true;
    }
  return false;
}

bool Chuck::dpadChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.dpad.padUp != currentData.dpad.padUp || lastData.dpad.padDown != currentData.dpad.padDown ||
    lastData.dpad.padRight != currentData.dpad.padRight || lastData.dpad.padLeft != currentData.dpad.padLeft) {
      return true;
    }
  return false;
}

bool Chuck::buttonsChanged() {
  lastData = this->lastData;
  currentData = this->currentData;
  if(lastData.buttons.buttonC != currentData.buttons.buttonC || lastData.buttons.buttonZ != currentData.buttons.buttonZ || 
    lastData.buttons.buttonX != currentData.buttons.buttonX || lastData.buttons.buttonY != currentData.buttons.buttonY ||
    lastData.buttons.buttonA != currentData.buttons.buttonA || lastData.buttons.buttonB != currentData.buttons.buttonB || 
    lastData.buttons.buttonMinus != currentData.buttons.buttonMinus || lastData.buttons.buttonHome != lastData.buttons.buttonHome || 
    lastData.buttons.buttonPlus != lastData.buttons.buttonPlus) {
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
      //if(this->valuesChanged()) 
      BLE.poll();
    }
    break;
    case WIFI: 
    {
      String type;
      MsgPack::Packer packer;

      if(this->infoChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckInfo",
          "data", this->currentData.info
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();        
      }

      if(this->axisChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckSticks",
          "data", this->currentData.sticks
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();  
      }

      if(this->acclChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckAccelerometer",
          "data", this->currentData.accelerometer
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();  
      }

      if(this->buttonsChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckButtons",
          "data", this->currentData.buttons
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();  
      }

      if(this->triggerChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckTriggers",
          "data", this->currentData.triggers
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();  
      }

      if(this->dpadChanged()) {
        packer.serialize(MsgPack::map_size_t(2), 
          "type", "ChuckDpad",
          "data", this->currentData.dpad
        );
        SendMsgPack(packer.data(), packer.size());
        packer.clear();  
      }
    }
    break;
  }
}

void Chuck::debugPrint() {
  Serial.print("X: "); Serial.print(this->currentData.accelerometer.acclX);
  Serial.print(" \tY: "); Serial.print(this->currentData.accelerometer.acclY); 
  Serial.print(" \tZ: ");  Serial.print(this->currentData.accelerometer.acclZ);

  Serial.print("\t\tLeft Joy: ("); 
  Serial.print(this->currentData.sticks.axisLeftX);
  Serial.print(", "); 
  Serial.print(this->currentData.sticks.axisLeftY);
  Serial.print(")");

  
  Serial.print("\t\tRight Joy: ("); 
  Serial.print(this->currentData.sticks.axisRightX);
  Serial.print(", "); 
  Serial.print(this->currentData.sticks.axisRightY);
  Serial.print(")");

  Serial.print("\t\tButton: "); 
  if (this->currentData.buttons.buttonZ) Serial.print(" Z "); 
  if (this->currentData.buttons.buttonC) Serial.print(" C "); 

  if (this->currentData.buttons.buttonA) Serial.print(" A "); 
  if (this->currentData.buttons.buttonB) Serial.print(" B "); 
  if (this->currentData.buttons.buttonX) Serial.print(" X "); 
  if (this->currentData.buttons.buttonY) Serial.print(" Y "); 

  Serial.println();
}
