#include <ArduinoBLE.h>

char TITLE[] = "CycleSense";
const int BAUD_SPEED = 9600;

const int HALL_PIN = 2;
const float REV_THRESHOLD = 1.0; // # of rotations before updating RPM
const int NUM_SAMPLES = 5;

unsigned long samples[NUM_SAMPLES];

const int DECAY_START_INTERVAL_MS = 1000;
const int DECAY_STEP_INTERVAL_MS = 500;

BLEService bikeService("97bb6403-1337-4a42-8563-243ed61234c7"); // BLE LED Service
BLEFloatCharacteristic bikeRPM("97bb6403-1338-4a42-8563-243ed61234c7", BLERead | BLENotify);

int rev;
float rps;
float rpm;
unsigned long timeold;
unsigned long timedecay;
int zerod = 0;

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

  // DELAY FOR STABILITY, REMOVE FOR PRODUCTION
  //delay(1);
}

void revCounterInterrupt()
{  
  rev++;
}

void processSensor() {
  float time_passed = (millis()-timeold);
  float time_decay = (millis()-timedecay);
  if (rev >= REV_THRESHOLD) {
    zerod = 0;

    detachInterrupt(HALL_PIN);

    // Calc most recent RPM
    rpm = 60000.0/(millis()-timeold)*rev;
    timeold = millis();
    timedecay = millis();
    rev = 0;

    //Serial.print("RPM-1s: ");
    //Serial.println(rpm);

    // Push latest RPM value to the beginning of the samples array
    samples[NUM_SAMPLES - 1] = samples[NUM_SAMPLES - 2];
    for (int i = NUM_SAMPLES - 2; i > 0; i--) {
      samples[i] = samples[i - 1];
    }
    samples[0] = rpm;

    // Calculate total RPM sum
    unsigned long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
      sum += samples[i];
    }

    unsigned long averageRPM = sum / NUM_SAMPLES; 
    
    //Serial.print("RPM-2s: ");
    //Serial.println(averageRPM);
    bikeRPM.writeValue(averageRPM);

    attachInterrupt(digitalPinToInterrupt(HALL_PIN),revCounterInterrupt,RISING);
  } else if(zerod < NUM_SAMPLES && time_passed > DECAY_START_INTERVAL_MS && time_decay > DECAY_STEP_INTERVAL_MS) {
    timedecay = millis();

    // If no revolutions occur after DECAY_START_INTERVAL_MS, 
    // we push zeros onto the samples every DECAY_STEP_INTERVAL_MS
    samples[NUM_SAMPLES - 1] = samples[NUM_SAMPLES - 2];
    for (int i = NUM_SAMPLES - 2; i > 0; i--) {
      samples[i] = samples[i - 1];
    }
    samples[0] = 0;

    unsigned long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
      sum += samples[i];
    }

    unsigned long averageRPM = sum / NUM_SAMPLES; 
    
    //Serial.print("CLEAR RPMs: ");
    //Serial.println(averageRPM);
    bikeRPM.writeValue(averageRPM);
     
    // Prevents us from the if from triggering after we've cleared all samples
    zerod++;
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