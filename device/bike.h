#ifndef BIKE_H
#define BIKE_H

const int DECAY_START_INTERVAL_MS = 1000;
const int DECAY_STEP_INTERVAL_MS = 500;

const int BIKE_PIN = 2;
const float REV_THRESHOLD = 1.0; // # of rotations before updating RPM
const int NUM_SAMPLES = 5; // # of calculated RPM samples to take average from

struct BikeData {
  float rpm;
};

class Bike {
  public:
    void initialize();
    void process(); 
  private:
    BikeData data;
    unsigned long timeold;
    unsigned long timedecay;
    int zerod = 0;

    unsigned long samples[NUM_SAMPLES];

    unsigned long calcAverageRPM();
    void pushSample(float);
    void sendData();
    static void revCounterInterrupt();
};


#endif