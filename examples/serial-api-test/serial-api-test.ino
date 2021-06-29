#include <Arduino.h>
#include <pas-co2-test-bb.hpp>
#include <pas-co2-pal-timer-ino.hpp>
#include <pas-co2-pal-gpio-ino.hpp>
#include <pas-co2-serial-ino.hpp>

/**
 * Select the serial interface:
 * - I2C (TwoWire)
 * - UART (HardwareSerial)
 * By default the I2C interfaces is selected. 
 * Compile with -DINO_HW_SERIAL to select the UART interface.
 */
#ifdef INO_HW_SERIAL
HardwareSerial * bus = (HardwareSerial*) pltf->uart;
#else
TwoWire * bus = (TwoWire*) pltf->i2c;
#endif

PASCO2SerialIno cotwo(bus, pltf->inte);

void setup()
{
  /**
   * Serial Initialization 
   */ 
  Serial.begin(9600);
  Serial.println("[arduino]       : serial initialized");

  TimerIno        t;
  test_serialAPI(cotwo, t);
  cotwo.~PASCO2SerialIno();
}

void loop()
{

}