#ifndef CONSTANTS_H
#define CONSTANTS_H

// Serial
const int BAUD_SPEED = 9600;

// Bluetooth
enum deviceConnType {BLUETOOTH, WIFI, USB};
const int CONNECTION_TYPE = WIFI;

// WiFi

// Not sure where to put these... 
const char ssid[] = "TP-LINKARINO"; 
const char pass[] = "dinbeandinbean";
const int udpServerPort = 1337; 

#endif