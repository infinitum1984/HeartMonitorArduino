#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "BleController.h"


MAX30105 particleSensor;

const byte RATE_SIZE = 32; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rate
float temps[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
long lastCheck = 0;
BleController ble;
float beatsPerMinute;
int beatAvg;
byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
byte ledMode = 1; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 69; //Options: 69, 118, 215, 411
int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
String bleData = String("");
void setup()
{

  ble.begin();

  Serial.begin(9600);
  Serial.println("Initializing...");
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(); //Configure sensor with default settings
}

void resetArray(){
  for(int i = 0; i< RATE_SIZE; i++){
    rates[i] = -1;
  }
}

bool rateArrayIsFill(){
  int numNotEmpty = 0;
  for(int i = 0; i< RATE_SIZE; i++){
    if(rates[i] != -1){
      numNotEmpty++;
    }
  }
  return numNotEmpty==RATE_SIZE;
}

void checkConnection(){
  if( millis()-lastCheck>1000){
    lastCheck = millis();
    Serial.println(digitalRead(BLE_STATE_PIN));
  }
}

void loop()
{
  ble.showState();
  long irValue = particleSensor.getIR();
  boolean isTouched = true;
  if (irValue < 50000){
    isTouched = false;
  }
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
        float temperature = particleSensor.readTemperature();
        temps[rateSpot] = temperature;
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable
        if(rateArrayIsFill()){
              beatAvg = 0;
              float tempAvg = 0.0;
        for (byte x = 0 ; x < RATE_SIZE ; x++){
          beatAvg += rates[x];
          tempAvg +=temps[x];
        }      
        beatAvg /= RATE_SIZE;
        tempAvg /= RATE_SIZE;
        ble.sendData(isTouched,beatAvg,tempAvg );
      }
    }
  }
}


