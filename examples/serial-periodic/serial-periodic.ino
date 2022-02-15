#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ  400000                     
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  7 

/*
 * Create CO2 object. Unless otherwise specified,
 * using the Wire interface
 */
PASCO2SerialIno cotwo;

int16_t co2ppm;
uint16_t pressureReference = 900;
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

    /* We can set the reference pressure before starting 
     * the measure 
     */
    err = cotwo.setPressRef(pressureReference);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }

    /*
     * Configure the sensor to measureme periodically 
     * every 7 seconds
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

    co2ppm = 0;

    err = cotwo.getCO2(co2ppm);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("get co2 error: ");
      Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);

    /*
     * Assuming we have some mechanism to obtain a
     * pressure reference (i.e. a pressure sensor),
     * we could compensate again by setting the new reference. 
     * Here we just keep the initial value.
     */
    err = cotwo.setPressRef(pressureReference);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }
}