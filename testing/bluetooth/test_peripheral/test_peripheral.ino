#include <ArduinoBLE.h>

char TITLE[] = "CycleSense";
int BAUD_SPEED = 9600;

int HALL_PIN = 2;
float REV_THRESHOLD = 1.0; // # of rotations before updating RPM

BLEService bikeService("97bb6403-1337-4a42-8563-243ed61234c7"); // BLE LED Service
BLEFloatCharacteristic bikeRPM("97bb6403-1338-4a42-8563-243ed61234c7", BLERead | BLENotify);

int rev;
float rps;
float rpm;
unsigned long timeold;

void setup() {
  Serial.begin(BAUD_SPEED);

  BLE.begin();
  BLE.setLocalName(TITLE);
  BLE.setDeviceName(TITLE);
  BLE.setAdvertisedService(bikeService);
  
  bikeService.addCharacteristic(bikeRPM);
  bikeRPM.writeValue(0);

  attachInterrupt(digitalPinToInterrupt(HALL_PIN),revCounterInterrupt,RISING);

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);
  BLE.addService(bikeService);
  BLE.advertise();

  Serial.println("CycleSense active, waiting for connections...");
}

void loop() {
  // Handles bluetooth events
  BLE.poll();

  processSensor();
}

void revCounterInterrupt()
{  
  rev++;
}

void processSensor() {
  float time_passed = (millis()-timeold);
  if (rev >= REV_THRESHOLD) {
    detachInterrupt(HALL_PIN);
    rpm = 60000.0/(millis()-timeold)*rev;
    timeold = millis();
    rev = 0;
    
    Serial.print("RPMs: ");
    Serial.println(rpm);
    bikeRPM.writeValue(rpm);

    attachInterrupt(digitalPinToInterrupt(HALL_PIN),revCounterInterrupt,RISING);
    
    // COMMENT DELAY FOR PRODUCTION
    //delay(1);
  } else if(rpm > 0 && time_passed > 10000) {
    rpm -= 0.1;
    bikeRPM.writeValue(rpm);    
  }
}

void connectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void disconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}