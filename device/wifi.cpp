#include "wifi.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "constants.h"
#include "bike.h"
#include "chuck.h"
#include "string.h"
#include <MsgPack.h>

// UDP
IPAddress currentIp;
int status = WL_IDLE_STATUS;
int keyIndex = 0;            // your network key Index number (needed only for WEP)
char packetBuffer[1024]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged"; 
WiFiUDP Udp;

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WifiSetup() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(udpServerPort);
}

void SendMessage(String typeName, StaticJsonDocument<200> doc) {
    if(currentIp) {
      MsgPack::Packer packer;
      doc["type"] = typeName;

      //debug
      //Serial.print("Sending data: ");
      //Serial.println(typeName);
      //serializeJson(doc, Serial);

      packer.serialize(doc);

      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(packer.data(), packer.size());
      Udp.endPacket();
    }
}

void ProcessWifi() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize != 0) {
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
	
	  // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, packetSize);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    if (strcmp(packetBuffer, HEARTBEAT_MSG) == 0) {
        // Send heartbeat, nothing fancy yet
        StaticJsonDocument<200> heartbeat;
        SendMessage("Heartbeat", heartbeat);
    } else if (strcmp(packetBuffer, WIFI_CONNECT_MSG) == 0) {
        // Received connect message, set app IP
        currentIp = Udp.remoteIP();
        Serial.print("Received CONNECT from ");
        Serial.print(currentIp);
        Serial.print(":");
        Serial.println(Udp.remotePort());      

        // Send acknowledgement
        StaticJsonDocument<200> ack;
        ack["msg"] = "Ready to motionlessly roll...";
        SendMessage("ConnectAck", ack);
    }
  }  
}

void WifiSendChuckDataMsgPack(ChuckData currentData) {
  MsgPack::Packer packer;
  //packer.pack(currentData.controllerType);

  StaticJsonDocument<200> sticks;
  sticks["axis_left_x"] = currentData.axisLeftX;
  sticks["axis_left_y"] = currentData.axisLeftY;
  sticks["axis_right_x"] = currentData.axisRightX;
  sticks["axis_right_y"] = currentData.axisRightY;

  StaticJsonDocument<200> buttons;
  buttons["button_c"] = currentData.buttonC;
  buttons["button_z"] = currentData.buttonZ;
  buttons["button_a"] = currentData.buttonA;
  buttons["button_b"] = currentData.buttonB;
  buttons["button_x"] = currentData.buttonX;
  buttons["button_y"] = currentData.buttonY;
  buttons["button_minus"] = currentData.buttonMinus;
  buttons["button_home"] = currentData.buttonHome;
  buttons["button_plus"] = currentData.buttonPlus;

  packer.serialize(sticks);
  packer.serialize(buttons);

  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(packer.data(), packer.size());
  Udp.endPacket();

  Serial.print("msgpack data size: ");
  Serial.println(packer.size());
}

// Wifi sensor handlers... refactor this to not be in the Wifi code?
void WifiSendChuckData(Chuck chuck) {
  ChuckData currentData = chuck.getCurrentData();

  // Build json doc object of current data and send over udp
  if(chuck.infoChanged()) {
    StaticJsonDocument<200> info;
    info["controller_type"] = currentData.controllerType;
    SendMessage("ChuckInfo", info);
  }

  if(chuck.axisChanged()) {
    StaticJsonDocument<200> sticks;
    sticks["axis_left_x"] = currentData.axisLeftX;
    sticks["axis_left_y"] = currentData.axisLeftY;
    sticks["axis_right_x"] = currentData.axisRightX;
    sticks["axis_right_y"] = currentData.axisRightY;
    SendMessage("ChuckSticks", sticks);
  }

  if(chuck.acclChanged()) {
    StaticJsonDocument<200> accelerometer;
    accelerometer["accl_x"] = currentData.acclX;
    accelerometer["accl_y"] = currentData.acclY;
    accelerometer["accl_Z"] = currentData.acclZ;
    SendMessage("ChuckAccelerometer", accelerometer);
  }

  if(chuck.buttonsChanged()) {
    StaticJsonDocument<200> buttons;
    buttons["button_c"] = currentData.buttonC;
    buttons["button_z"] = currentData.buttonZ;
    buttons["button_a"] = currentData.buttonA;
    buttons["button_b"] = currentData.buttonB;
    buttons["button_x"] = currentData.buttonX;
    buttons["button_y"] = currentData.buttonY;
    buttons["button_minus"] = currentData.buttonMinus;
    buttons["button_home"] = currentData.buttonHome;
    buttons["button_plus"] = currentData.buttonPlus;
    SendMessage("ChuckButtons", buttons);
  }

  if(chuck.triggerChanged()) {
    StaticJsonDocument<200> triggers;
    triggers["trigger_left"] = currentData.triggerLeft;
    triggers["trigger_right"] = currentData.triggerRight;
    triggers["trigger_z_left"] = currentData.triggerZLeft;
    triggers["trigger_z_right"] = currentData.triggerZRight;
    SendMessage("ChuckTriggers", triggers);
  }

  if(chuck.dpadChanged()) {
    StaticJsonDocument<200> dpad;
    dpad["pad_up"] = currentData.padUp;
    dpad["pad_down"] = currentData.padDown;
    dpad["pad_right"] = currentData.padRight;
    dpad["pad_left"] = currentData.padLeft;
    SendMessage("ChuckDpad", dpad);
  }
}

void WifiProcessBike(BikeData data) {
    StaticJsonDocument<200> bike;
    bike["rpm"] = data.rpm;
    SendMessage("Bike", bike);
}
