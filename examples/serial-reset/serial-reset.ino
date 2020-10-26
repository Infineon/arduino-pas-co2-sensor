#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

PASCO2SerialIno cotwo;

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");

  Error_t err = cotwo.reset();
  if(OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.println("software reset completed");
}

void loop()
{

}