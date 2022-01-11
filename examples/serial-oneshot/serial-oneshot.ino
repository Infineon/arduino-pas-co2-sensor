#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

/**
 * Select the serial interface:
 * - I2C (TwoWire)
 * - UART (HardwareSerial)
 * By default the I2C interfaces is selected. 
 * Compile with -DINO_HW_SERIAL to select the UART interface.
 */
#ifdef INO_HW_SERIAL
HardwareSerial * bus = (HardwareSerial*) PASCO2_INO_UART;
#else
TwoWire * bus = (TwoWire*) PASCO2_INO_I2C;
#endif

PASCO2SerialIno cotwo(bus);

int16_t co2ppm;
Error_t err;

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("pas co2 serial initialized");

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
   *  result is yet available. It returns a 
   *  negative value in case of error.
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