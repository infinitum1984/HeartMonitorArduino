/*
  Optical Heart Rate Detection (PBA Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This is a demo to show the reading of heart rate or beats per minute (BPM) using
  a Penpheral Beat Amplitude (PBA) algorithm.

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>

#define LED_PIN 7
#define BLE_STATE_PIN 4
MAX30105 particleSensor;

const byte RATE_SIZE = 32; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rate
float temps[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
long lastCheck = 0;
SoftwareSerial blePort(8,9);
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
  pinMode(LED_PIN, OUTPUT);
  pinMode(BLE_STATE_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Initializing...");
  blePort.begin(9600);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
  while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
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
  long irValue = particleSensor.getIR();
  boolean isTouched = true;
  digitalWrite(LED_PIN, digitalRead(BLE_STATE_PIN));
  if (irValue < 50000){
    //Serial.println(" No finger?");
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
      //Take average of readings
      if(rateArrayIsFill()){

              beatAvg = 0;
              float tempAvg = 0.0;

      for (byte x = 0 ; x < RATE_SIZE ; x++){
        beatAvg += rates[x];
        tempAvg +=temps[x];
      }

      
      beatAvg /= RATE_SIZE;
      tempAvg /= RATE_SIZE;
      bleData =String("")+isTouched+","+beatAvg+","+tempAvg;

      blePort.print(bleData);
      Serial.println(bleData);
      }
    }
  }

  }


