#include <Arduino.h>
#include <pas-co2-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ 400000
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define FORCED_COMPENSATION_CO2_REFERENCE  400
#define PRESSURE_REFERENCE  900

/**
 * Create CO2 object. Unless otherwise specified,
 * using the Wire interface
 */
PASCO2Ino cotwo;

int16_t co2ppm;
Error_t err;

void setup()
{
    Serial.begin(9600);
    delay(500);
    Serial.println("serial initialized");

    /* Initialize the i2c interface used by the sensor */
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

  err = cotwo.setPressRef(PRESSURE_REFERENCE);
  if (XENSIV_PASCO2_OK != err)
  {
    Serial.print("pressure reference error: ");
    Serial.println(err);
  }

    Serial.print("forced compensation started with ");
    Serial.print(FORCED_COMPENSATION_CO2_REFERENCE);
    Serial.println(" ppm baseline reference ...");

    err = cotwo.performForcedCompensation(FORCED_COMPENSATION_CO2_REFERENCE);
    if (XENSIV_PASCO2_OK != err)
    {
      Serial.print("forced compensation error: ");
      Serial.println(err);
    }

    Serial.println("forced compensation finished");

    /*
      * Configure the sensor to measureme periodically 
      * every 10 seconds
      */
    err = cotwo.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }
}

void loop()
{
    /* Wait for the value to be ready. */
    delay(PERIODIC_MEAS_INTERVAL_IN_SECONDS*1000);

    err = cotwo.getCO2(co2ppm);
    if(XENSIV_PASCO2_OK != err)
    {
      /* Retry in case of timing synch mismatch */
      if(XENSIV_PASCO2_ERR_COMM == err)
      {
        delay(600);
        err = cotwo.getCO2(co2ppm);
        if(XENSIV_PASCO2_OK != err)          
        {
          Serial.print("get co2 error: ");
          Serial.println(err);
        }
      }
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);

    /*
     * Assuming we have some mechanism to obtain a
     * pressure reference (i.e. a pressure sensor),
     * we could compensate again by setting the new reference. 
     * Here we just keep the initial value.
     */
    err = cotwo.setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }
}