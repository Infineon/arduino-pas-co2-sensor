#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
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

  /* Initialize the sensor */
  err = cotwo.begin();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(err);
  }

  /* Clear previous forced compensation offset */
  err = cotwo.clearForcedCompensation();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("clear forced compensation error: ");
    Serial.println(err);
  }

  uint16_t co2ref = 800;
  err = cotwo.performForcedCompensation(co2ref);
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("forced compensation error: ");
    Serial.println(err);
  }

}

void loop()
{

}