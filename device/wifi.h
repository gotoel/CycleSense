#ifndef WIFI_H
#define WIFI_H

#include "bike.h"
#include "chuck.h"

const char WIFI_CONNECT_MSG[] = "CYCLESENSE|CONNECT";
const char HEARTBEAT_MSG[] = "Heartbeat";

void WifiSetup();
void ProcessWifi();
void WifiSendChuckData(ChuckData chuck);
void WifiProcessBike(BikeData bike);

#endif