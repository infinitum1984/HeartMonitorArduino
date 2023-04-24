#include "BleDevice.h"
BleDevice::BleDevice(){
  blePort = new SoftwareSerial(8,9);
}
void BleDevice::begin(){
    pinMode(BLE_STATE_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    blePort->begin(9600);
}
void BleDevice::sendData(String data){
    if(digitalRead(BLE_STATE_PIN)){
      blePort->print(data);
      Serial.println(data);
      digitalWrite(LED_PIN, 1);
    }else{
      digitalWrite(LED_PIN, 0);
    }   
}
