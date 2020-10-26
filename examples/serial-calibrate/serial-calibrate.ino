#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

PASCO2SerialIno cotwo;
ABOC_t   aboc     = ABOC_ENABLED_PERIODIC;
int16_t  abocRef  = 100;
uint16_t pressRef = 700;

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");

  Error_t err = cotwo.calibrate(aboc, abocRef, pressRef);
  if(OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.print("sensor calibration completed");
}

void loop()
{

}