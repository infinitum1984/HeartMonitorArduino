#ifndef BleDevice_h
#define BleDevice_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#define BLE_STATE_PIN 5
#define LED_PIN 4

class BleDevice{
  private:
    SoftwareSerial *blePort;

  public:
    BleDevice();
    void sendData(String data);
    void begin();
};
#endif
