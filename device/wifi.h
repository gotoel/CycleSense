#ifndef WIFI_H
#define WIFI_H

#include "bike.h"
#include "chuck.h"

const char WIFI_CONNECT_MSG[] = "CYCLESENSE|CONNECT";

void WifiSetup();
void ProcessWifi();
void WifiProcessChuck(ChuckData lastData, ChuckData currentData);
void WifiProcessBike(BikeData bike);

#endif