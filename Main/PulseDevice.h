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
    byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
    byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 1; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 69; //Options: 69, 118, 215, 411
    int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
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
