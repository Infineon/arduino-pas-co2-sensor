#include <Arduino.h>
#include <pas-co2-pulse-ino.hpp>

PASCO2PulseIno cotwo(pltf->pwm);

int16_t co2ppm;
Error_t err;

void setup()
{
    Serial.begin(9600);
    Serial.println("pas co2 serial initialized");

    err = cotwo.startMeasure();
    if(pasco2::OK != err)
    {
        Serial.print("start measure error: ");
        Serial.println(err);
    }
}

void loop()
{
    err = cotwo.getCO2(co2ppm);
    if(pasco2::OK != err)
    {
        Serial.print("get co2 error: ");
        Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);
}