#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

#define I2C_FREQ_HZ 400000  

/**
 * Create CO2 object. Unless otherwise specified,
 * using the Wire interface
 */
PASCO2SerialIno cotwo;

int16_t co2ppm;
Error_t err;

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("serial initialized");

  /* Initialize the i2c serial interface used by the sensor */
  Wire.begin();
  Wire.setClock(I2C_FREQ_HZ);

  err = cotwo.begin();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(err);
  }

}

void loop()
{

  /* 
   * Trigger the measure.
   */
  err = cotwo.startMeasure();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  /* Wait for the value to be ready. */
  delay(5000);

  co2ppm = 0;

  /**
   *  getCO2() is called until the value is 
   *  available.  
   *  getCO2() returns 0 when no measurement 
   *  result is yet available or an error has
   *  occurred.
   */

  do
  {
    err = cotwo.getCO2(co2ppm);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("error: ");
      Serial.println(err);
      break;
    }
  } while (0 == co2ppm);

  Serial.print("co2 ppm value : ");
  Serial.println(co2ppm);
}