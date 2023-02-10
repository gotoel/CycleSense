#include "wifi.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "constants.h"
#include "bike.h"
#include "chuck.h"
#include "string.h"

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
    delay(2000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(udpServerPort);
}

void SendMsgPack(const uint8_t* data, const size_t size) {
   if(currentIp) {
      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(data, size);
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
      MsgPack::Packer packer;
      // Send heartbeat, nothing fancy yet
      packer.serialize(MsgPack::map_size_t(1), 
        "type", "Heartbeat"
      );
      SendMsgPack(packer.data(), packer.size());
    } else if (strcmp(packetBuffer, WIFI_CONNECT_MSG) == 0) {
      MsgPack::Packer packer;
      // Received connect message, set app IP
      currentIp = Udp.remoteIP();
      Serial.print("Received CONNECT from ");
      Serial.print(currentIp);
      Serial.print(":");
      Serial.println(Udp.remotePort());      

      // Send acknowledgement
      packer.serialize(MsgPack::map_size_t(2), 
        "type", "ConnectAck",
        "data", "Ready to motionlessly roll..."
      );
      SendMsgPack(packer.data(), packer.size());
    }
  }  
}