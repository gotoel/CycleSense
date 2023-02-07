#ifndef CHUCK_H
#define CHUCK_H

#include "Arduino.h"

struct ChuckData {
  unsigned char controllerType;

  // Wii Nunchuk
  unsigned char   acclX;
  unsigned char   acclY;
  unsigned char   acclZ;

  unsigned char  axisLeftX;
  unsigned char  axisLeftY;
  unsigned char  axisRightX;
  unsigned char  axisRightY;

  bool buttonC;
  bool buttonZ;

  bool buttonA;
  bool buttonB;
  bool buttonX;
  bool buttonY;

  bool triggerZLeft;
  unsigned char triggerLeft;
  unsigned char triggerRight;
  bool triggerZRight;

  bool buttonMinus;
  bool buttonHome;
  bool buttonPlus;

  bool padLeft;
  bool padUp;
  bool padRight;
  bool padDown;
};

class Chuck {
  public:
    void initialize();
    void process(); 
    bool valuesChanged();
    ChuckData getCurrentData();
  private:
    ChuckData currentData;
    ChuckData lastData;
  
    ChuckData collectData();
    void sendData();
    void debugPrint();
};


#endif