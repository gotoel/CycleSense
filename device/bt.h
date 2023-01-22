#ifndef BT_H
#define BT_H

#include "bike.h"
#include "chuck.h"

const char BT_TITLE[] = "CycleSense";

void BTSetup();
void BTProcessChuck(ChuckData lastData, ChuckData currentData);
void BTProcessBike(BikeData bike);
void connectHandler(BLEDevice central);
void disconnectHandler(BLEDevice central);

#endif