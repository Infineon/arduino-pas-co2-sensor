#include <Arduino.h>
#include <pas-co2-test-bb.hpp>
#include <pas-co2-pal-timer-ino.hpp>
#include <pas-co2-pal-gpio-ino.hpp>
#include <pas-co2-serial-ino.hpp>

/* The interrupt pin definition is defined according to the hardware
This will be moved to the pas-co2-platf-ino.hpp/cpp */

#ifdef XMC1100_XMC2GO                                                   
uint8_t intPin = 9;
#else
uint8_t intPin = GPIOIno::unusedPin;
#endif

void setup()
{
  /**
   * Serial Initialization 
   */ 
  Serial.begin(115200);
  Serial.println("[arduino]       : serial initialized");

  /**
   * Serial API (I2C) Test
   */
  TimerIno        t;
  PASCO2SerialIno cotwo(nullptr, intPin);
  test_serialAPI(cotwo, t);
  cotwo.~PASCO2SerialIno();
}

void loop()
{

}