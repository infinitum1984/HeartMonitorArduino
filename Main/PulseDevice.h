#ifndef PulseDevice_h
#define PulseDevice_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "PulseData.h"

class PulseDevice{
  private:
    MAX30105 particleSensor;
    static const byte RATE_SIZE = 32; //Increase this for more averaging. 4 is good.
    byte rates[RATE_SIZE]; //Array of heart rate
    float temps[RATE_SIZE];
    byte rateSpot = 0;
    long lastBeat = 0; //Time at which the last beat occurred
    float beatsPerMinute;
    int beatAvg;
    boolean ready;
    PulseData *pulseData;
    void resetArray();
    bool rateArrayIsFill();
  public:
    PulseDevice();
    void begin();
    void update();
    boolean isReady();
    PulseData getData();
};
#endif
