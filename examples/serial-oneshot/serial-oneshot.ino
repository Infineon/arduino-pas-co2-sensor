#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

PASCO2SerialIno cotwo;
int16_t co2ppm;
Error_t err;

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");
}

void loop()
{

  /* Trigger the measure with startMeasure() */
  err = cotwo.startMeasure();
  if(OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  /* Wait for the value to be ready */
  delay(5000);

  co2ppm = 0;

  /**
   *  getCO2() is called until the value is 
   *  available.  
   *  getCO2() returns 0 when no measurement 
   *  result is yet avaiable. If returns a 
   *  negative value if case of error.
   */

  do
  {
    err = cotwo.getCO2(co2ppm);
    if(OK != err)
    {
      Serial.print("error: ");
      Serial.println(err);
      break;
    }
  } while (0 == co2ppm);

  Serial.print("co2 ppm value : ");
  Serial.println(co2ppm);
}