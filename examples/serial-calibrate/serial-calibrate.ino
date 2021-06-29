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
HardwareSerial * bus = (HardwareSerial*) pltf->uart;
#else
TwoWire * bus = (TwoWire*) pltf->i2c;
#endif

PASCO2SerialIno cotwo(bus);

ABOC_t   aboc     = ABOC_ENABLED_PERIODIC;
int16_t  abocRef  = 100;
uint16_t pressRef = 700;

void setup()
{
  Serial.begin(9600);
  Serial.println("pas co2 serial initialized");

  Error_t err = cotwo.calibrate(aboc, abocRef, pressRef);
  if(pasco2::OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.print("sensor calibration completed");
}

void loop()
{

}