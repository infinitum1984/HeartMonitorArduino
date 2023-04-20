#ifndef BleController_h
#define BleController_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#define BLE_STATE_PIN 5
#define LED_PIN 4

class BleController{
  private:
    SoftwareSerial *blePort;

  public:
    BleController();
    void sendData(boolean isTouched, int beatAvg, int tempAvg);
    void showState();
    void begin();
};
#endif
