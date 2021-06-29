#include <Arduino.h>
#include <pas-co2-pulse-ino.hpp>

PASCO2PulseIno cotwo(pltf->pwm);

int16_t co2ppm;
Error_t err;

void setup()
{
    Serial.begin(9600);
    Serial.println("pas co2 serial initialized");
}

void loop()
{
    err = cotwo.reset();
    if(pasco2::OK != err)
    {
        Serial.print("reset error: ");
        Serial.println(err);
    }
}