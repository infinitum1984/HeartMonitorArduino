#include "BleDevice.h"
#include "PulseData.h"
#include "PulseDevice.h"
BleDevice ble;
PulseDevice pulseDevice;

void setup()
{
  Serial.begin(9600);
  ble.begin();
  pulseDevice.begin();
}

void loop()
{
  pulseDevice.update();
  if(pulseDevice.isReady()){
    ble.sendData(pulseDevice.getData().serialized());
  }
}


