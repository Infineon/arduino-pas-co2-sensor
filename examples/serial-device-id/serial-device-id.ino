#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

PASCO2SerialIno cotwo;
uint8_t prodId, revId;

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");
  
  Error_t err = cotwo.getDeviceID(prodId, revId);
  if(OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.print("product id  : ");
  Serial.println(prodId);
  Serial.print("revision id : ");
  Serial.println(revId);
}

void loop()
{

}