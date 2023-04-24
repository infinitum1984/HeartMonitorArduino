#ifndef PulseData_h
#define PulseData_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>
class PulseData{
    public:
      PulseData(boolean isTouched, int beatAvg, int tempAvg);
      boolean isTouched;
      int beatAvg;
      int tempAvg;
      String serialized();
};
#endif
