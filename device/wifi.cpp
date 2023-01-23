#include "wifi.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "constants.h"
#include "bike.h"
#include "chuck.h"


// UDP
IPAddress currentIp;
int status = WL_IDLE_STATUS;
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned int localPort = 1337;      // local port to listen on
char packetBuffer[256]; //buffer to hold incoming packet
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
  Udp.begin(localPort);
}

void ProcessWifi() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize != 0) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("Size of ");
    Serial.println(sizeof(WIFI_CONNECT_MSG));
	
	  // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, packetSize);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    // Received connect message, set app IP
    if(strcmp(packetBuffer,WIFI_CONNECT_MSG) == 0) {
        currentIp = Udp.remoteIP();
        Serial.print("Received CONNECT from ");
        Serial.print(currentIp);
        Serial.print(":");
        Serial.println(Udp.remotePort());      
    }
  }  
}

void SendMessage(String typeName, StaticJsonDocument<200> doc) {
    if(currentIp) {
      doc["type"] = typeName;

      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write(ReplyBuffer);
      serializeJson(doc, Udp);
      Udp.endPacket();
    }
}

void WifiProcessChuck(ChuckData lastData, ChuckData currentData) {
  // I don't like this...
  if(lastData.acclX != currentData.acclX || lastData.acclY != currentData.acclY ||
    lastData.acclZ != currentData.acclZ || lastData.axisX != currentData.axisX ||
    lastData.axisY != currentData.axisY || lastData.buttonC != currentData.buttonC ||
    lastData.buttonZ != currentData.buttonZ) {
      // Build json doc object of current data and send over udp
      StaticJsonDocument<200> doc;
      doc["accl_x"] = currentData.acclX;
      doc["accl_y"] = currentData.acclY;
      doc["accl_Z"] = currentData.acclZ;
      doc["axis_x"] = currentData.axisX;
      doc["axis_y"] = currentData.axisY;
      doc["button_c"] = currentData.buttonC;
      doc["button_z"] = currentData.buttonZ;
      SendMessage("Chuck", doc);
  }
}

void WifiProcessBike(BikeData data) {
    StaticJsonDocument<200> doc;
    doc["rpm"] = data.rpm;
    SendMessage("Bike", doc);
}
