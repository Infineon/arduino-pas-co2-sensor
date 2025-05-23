#include <Arduino.h>
#include <pas-co2-ino.hpp>

#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define PRESSURE_REFERENCE  900

/*
 * Create CO2 object
 * Hardware serial Boards list: 
 * Arduino UNO R4 Wifi
 * Arduino Mega 2560
 * Arduino Leonardo
 * ESP32
 * Arduino MKR 1000 WiFi
 * XMC4700 Relax Kit
 */
PASCO2Ino cotwo(&Serial1);     


int16_t co2ppm;
Error_t err;

void setup()
{
    Serial1.begin(9600);
    Serial.begin(9600);
    delay(800);
    Serial.println("serial initialized");

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
    err = cotwo.setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }

    /*
     * Configure the sensor to measureme periodically 
     * every 60 seconds
     */
    err = cotwo.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }

    delay(1000);
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