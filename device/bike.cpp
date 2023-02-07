#include <ArduinoBLE.h>
#include "bike.h" 
#include "bt.h"
#include "wifi.h"
#include "constants.h"

int rev;

void Bike::initialize() {
  attachInterrupt(digitalPinToInterrupt(BIKE_PIN),revCounterInterrupt, RISING);
}

void Bike::process() {
  float time_passed = (millis()-this->timeold);
  float time_decay = (millis()-this->timedecay);
  if (rev >= REV_THRESHOLD) {
    this->zerod = 0;

    detachInterrupt(BIKE_PIN);

    // Calc most recent RPM
    this->data.rpm = 60000.0/(millis()-this->timeold)*rev;
    this->timeold = millis();
    this->timedecay = millis();
    rev = 0;

    // Push latest RPM value to the beginning of the samples array
    pushSample(this->data.rpm);

    unsigned long averageRPM = this->calcAverageRPM();
    
    this->data.rpm = averageRPM;

    this->sendData();

    attachInterrupt(digitalPinToInterrupt(BIKE_PIN),this->revCounterInterrupt, RISING);
  } else if(this->zerod < NUM_SAMPLES && time_passed > DECAY_START_INTERVAL_MS && time_decay > DECAY_STEP_INTERVAL_MS) {
    this->timedecay = millis();

    // If no revolutions occur after DECAY_START_INTERVAL_MS, 
    // we push zeros onto the samples every DECAY_STEP_INTERVAL_MS
    pushSample(0);
    unsigned long averageRPM = this->calcAverageRPM();
    this->data.rpm = averageRPM;

    this->sendData();
     
    // Prevents us from the if from triggering after we've cleared all samples
    this->zerod++;
	}
}


// Sends updated data down selected connection type
void Bike::sendData() {
  switch(CONNECTION_TYPE) {
    case BLUETOOTH:
      BTProcessBike(this->data);
      break;
    case WIFI:
      WifiProcessBike(this->data);
      break;
  }
}

unsigned long Bike::calcAverageRPM() {
    unsigned long sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
      sum += samples[i];
    }

    unsigned long averageRPM = sum / NUM_SAMPLES;

    return averageRPM; 
}

void Bike::pushSample(float rpm) {
    this->samples[NUM_SAMPLES - 1] = this->samples[NUM_SAMPLES - 2];
    for (int i = NUM_SAMPLES - 2; i > 0; i--) {
      this->samples[i] = this->samples[i - 1];
    }
    this->samples[0] = rpm;
}

void Bike::revCounterInterrupt()
{  
  rev++;
}