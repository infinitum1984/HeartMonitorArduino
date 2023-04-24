#include "PulseDevice.h"

PulseDevice::PulseDevice(){

}

void PulseDevice::begin(){
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(); //Configure sensor with default settings
}

void PulseDevice::update(){
  if(pulseData != NULL){
    delete pulseData;
    pulseData = NULL;
  }
  long irValue = particleSensor.getIR();
  boolean isTouched = true;
  ready = false;
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
          pulseData = new PulseData(isTouched,beatAvg,tempAvg);
          ready = true;
      }
    }
  }
}

boolean PulseDevice::isReady(){
  return ready;
}

PulseData PulseDevice::getData(){
  return (*pulseData);
}

void PulseDevice::resetArray(){
  for(int i = 0; i< RATE_SIZE; i++){
    rates[i] = -1;
  }
}

bool PulseDevice::rateArrayIsFill(){
  int numNotEmpty = 0;
  for(int i = 0; i< RATE_SIZE; i++){
    if(rates[i] != -1){
      numNotEmpty++;
    }
  }
  return numNotEmpty==RATE_SIZE;
}
