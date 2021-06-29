#include <Arduino.h>
#include <pas-co2-test-bb.hpp>
#include <pas-co2-pal-timer-ino.hpp>
#include <pas-co2-pulse-ino.hpp>

PASCO2PulseIno cotwo(pltf->pwm, pltf->inte);

void setup()
{
  /**
   * Serial Initialization 
   */ 
  Serial.begin(9600);
  Serial.println("[arduino]       : serial initialized");

  TimerIno        t;
  test_pulseAPI(cotwo, t);
  cotwo.~PASCO2PulseIno();
}

void loop()
{

}