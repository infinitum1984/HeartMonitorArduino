#include "PulseData.h";

PulseData::PulseData(boolean _isTouched, int _beatAvg, int _tempAvg){
  isTouched = _isTouched;
  beatAvg = _beatAvg;
  tempAvg = _tempAvg;
}
String PulseData::serialized(){
  return String("")+isTouched+","+beatAvg+","+tempAvg;
}