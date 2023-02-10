#ifndef CHUCK_H
#define CHUCK_H

#include "Arduino.h"
#include <MsgPack.h>

struct ChuckInfo {
  MsgPack::str_t key_controllerType {"controller_type"}; unsigned char controllerType;
  MSGPACK_DEFINE_MAP(key_controllerType, controllerType);
};

struct ChuckAccelerometer {
  MsgPack::str_t key_acclX {"accl_x"}; unsigned char acclX;
  MsgPack::str_t key_acclY {"accl_y"}; unsigned char acclY;
  MsgPack::str_t key_acclZ {"accl_z"}; unsigned char acclZ;
  MSGPACK_DEFINE_MAP(key_acclX, acclX, key_acclY, acclY, key_acclZ, acclZ);
};

struct ChuckSticks {
  MsgPack::str_t key_axisLeftX {"axis_left_x"}; unsigned char axisLeftX;
  MsgPack::str_t key_axisLeftY {"axis_left_y"}; unsigned char axisLeftY;
  MsgPack::str_t key_axisRightX {"axis_right_x"}; unsigned char axisRightX;
  MsgPack::str_t key_axisRightY {"axis_right_y"}; unsigned char axisRightY;
  MSGPACK_DEFINE_MAP(key_axisLeftX, axisLeftX, key_axisLeftY, axisLeftY,
                     key_axisRightX, axisRightX, key_axisRightY, axisRightY);
};

struct ChuckButtons {
  MsgPack::str_t key_axisLeftX {"axis_left_x"}; unsigned char axisLeftX;
  MsgPack::str_t key_axisLeftY {"axis_left_y"}; unsigned char axisLeftY;
  MsgPack::str_t key_axisRightX {"axis_right_x"}; unsigned char axisRightX;
  MsgPack::str_t key_axisRightY {"axis_right_y"}; unsigned char axisRightY;


  MsgPack::str_t key_buttonC {"button_c"}; bool buttonC;
  MsgPack::str_t key_buttonZ {"button_Z"}; bool buttonZ;

  MsgPack::str_t key_buttonA {"button_A"}; bool buttonA;
  MsgPack::str_t key_buttonB {"button_B"}; bool buttonB;
  MsgPack::str_t key_buttonX {"button_X"}; bool buttonX;
  MsgPack::str_t key_buttonY {"button_Y"}; bool buttonY;

  MsgPack::str_t key_buttonMinus {"button_minus"}; bool buttonMinus;
  MsgPack::str_t key_buttonHome {"button_home"}; bool buttonHome;
  MsgPack::str_t key_buttonPlus {"button_plus"}; bool buttonPlus;
  MSGPACK_DEFINE_MAP(key_buttonC, buttonC, key_buttonZ, buttonZ,
                     key_buttonA, buttonA, key_buttonB, buttonB,
                     key_buttonC, buttonX, key_buttonC, buttonY,
                     key_buttonMinus, buttonMinus, key_buttonHome, buttonHome,
                     key_buttonPlus, buttonPlus);
};

struct ChuckTriggers {
  MsgPack::str_t key_triggerLeft {"trigger_left"}; bool triggerLeft;
  MsgPack::str_t key_triggerRight {"trigger_right"}; bool triggerRight;
  MsgPack::str_t key_triggerZLeft {"trigger_z_left"}; bool triggerZLeft;
  MsgPack::str_t key_triggerZRight {"trigger_z_right"}; bool triggerZRight;

  MSGPACK_DEFINE_MAP(key_triggerLeft, triggerLeft, key_triggerRight, triggerRight,
                     key_triggerZLeft, triggerZLeft, key_triggerZRight, triggerZRight);
};

struct ChuckDpad {
  MsgPack::str_t key_padLeft {"pad_left"}; bool padLeft;
  MsgPack::str_t key_padRight {"pad_right"}; bool padRight;
  MsgPack::str_t key_padUp {"pad_up"}; bool padUp;
  MsgPack::str_t key_padDown {"pad_down"}; bool padDown;

  MSGPACK_DEFINE_MAP(key_padLeft, padLeft, key_padRight, padRight,
                     key_padUp, padUp, key_padDown, padDown);
};

struct ChuckData {
  ChuckInfo info;
  ChuckAccelerometer accelerometer;
  ChuckSticks sticks;
  ChuckButtons buttons;
  ChuckTriggers triggers;
  ChuckDpad dpad;
};

class Chuck {
  public:
    void initialize();
    void process(); 

    bool infoChanged();
    bool acclChanged();
    bool axisChanged();
    bool triggerChanged();
    bool dpadChanged();
    bool buttonsChanged();

    ChuckData getCurrentData();
  private:
    ChuckData currentData;
    ChuckData lastData;
  
    void collectData();
    void sendData();
    void debugPrint();
};


#endif