#include "BleController.h"
BleController::BleController(){
  blePort = new SoftwareSerial(8,9);
}
void BleController::begin(){
    pinMode(BLE_STATE_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    blePort->begin(9600);
}
void BleController::showState(){
  digitalWrite(LED_PIN, digitalRead(BLE_STATE_PIN));
}
void BleController::sendData(bool isTouched, int beatAvg, int tempAvg){
    String bleData =String("")+isTouched+","+beatAvg+","+tempAvg;
    blePort->print(bleData);
    Serial.println(bleData);
}
