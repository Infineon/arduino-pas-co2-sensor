#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

PASCO2SerialIno cotwo;
Diag_t diagnosis;

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");
  
  Error_t err = cotwo.getDiagnosis(diagnosis);
  if(OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.println("Diagnosis flags (1) true, (0) false : \n");
  Serial.print("sensor ready              : ");
  Serial.println(diagnosis.sensorRdy);
  Serial.print("pwm pin enabled           : ");
  Serial.println(diagnosis.pwmPinEnabled);
  Serial.print("temperature ouf of range  : ");
  Serial.println(diagnosis.outOfRangeTempError);
  Serial.print("ir emitter v out of range : ");
  Serial.println(diagnosis.outOfRange12VError);
  Serial.print("communication error       : ");
  Serial.println(diagnosis.commError);

}

void loop()
{

}