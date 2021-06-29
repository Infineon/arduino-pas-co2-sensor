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

PASCO2SerialIno cotwo(bus, pltf->inte);

int16_t co2ppm;
Error_t err;

volatile bool intFlag = false;
void isr (void * )
{
  intFlag = true;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("pas co2 serial initialized");

  err = cotwo.startMeasure(7, 0, isr);
  if(pasco2::OK != err)
  {
    Serial.print("start measure error: ");
    Serial.println(err);
  }
}

void loop()
{
    while(false == intFlag) { };

    Serial.println("int occurred");
    intFlag = false;

    err = cotwo.getCO2(co2ppm);
    if(pasco2::OK != err)
    {
      Serial.print("get co2 error: ");
      Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);
}