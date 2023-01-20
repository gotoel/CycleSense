#include <ArduinoBLE.h>
#include "hall.h" 

BLEService bikeService("97bb6403-1337-4a42-8563-243ed61234c7");
BLEFloatCharacteristic bikeRPM("97bb6403-1338-4a42-8563-243ed61234c7", BLERead | BLENotify);

int rev;

void Hall::initialize() {
  bikeService.addCharacteristic(bikeRPM);
  bikeRPM.writeValue(0);

  BLE.setAdvertisedService(bikeService);
  BLE.addService(bikeService);

  attachInterrupt(digitalPinToInterrupt(HALL_PIN),revCounterInterrupt,RISING);
}

void Hall::process() {
  float time_passed = (millis()-this->timeold);
  float time_decay = (millis()-this->timedecay);
  if (rev >= REV_THRESHOLD) {
    this->zerod = 0;

    detachInterrupt(HALL_PIN);

    // Calc most recent RPM
    this->rpm = 60000.0/(millis()-this->timeold)*rev;
    this->timeold = millis();
    this->timedecay = millis();
    rev = 0;

    // Push latest RPM value to the beginning of the samples array
    pushSample(this->rpm);

    unsigned long averageRPM = this->calcAverageRPM();
    bikeRPM.writeValue(averageRPM);

    attachInterrupt(digitalPinToInterrupt(HALL_PIN),this->revCounterInterrupt,RISING);
  } else if(this->zerod < NUM_SAMPLES && time_passed > DECAY_START_INTERVAL_MS && time_decay > DECAY_STEP_INTERVAL_MS) {
    this->timedecay = millis();

    // If no revolutions occur after DECAY_START_INTERVAL_MS, 
    // we push zeros onto the samples every DECAY_STEP_INTERVAL_MS
    pushSample(0);
    unsigned long averageRPM = this->calcAverageRPM();
    bikeRPM.writeValue(averageRPM);
     
    // Prevents us from the if from triggering after we've cleared all samples
    this->zerod++;
	}
}

unsigned long Hall::calcAverageRPM() {
    unsigned long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
      sum += samples[i];
    }

    unsigned long averageRPM = sum / NUM_SAMPLES;

    return averageRPM; 
}

void Hall::pushSample(float rpm) {
    this->samples[NUM_SAMPLES - 1] = this->samples[NUM_SAMPLES - 2];
    for (int i = NUM_SAMPLES - 2; i > 0; i--) {
      this->samples[i] = this->samples[i - 1];
    }
    this->samples[0] = rpm;
}

void Hall::revCounterInterrupt()
{  
  rev++;
}