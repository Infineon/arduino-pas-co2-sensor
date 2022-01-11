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
HardwareSerial * bus = (HardwareSerial*) PASCO2_INO_UART;
#else
TwoWire * bus = (TwoWire*) PASCO2_INO_I2C;
#endif

PASCO2SerialIno cotwo(bus, PASCO2_INO_INT);

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
  delay(500);
  Serial.println("pas co2 serial initialized");

  err = cotwo.begin();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(err);
  }

  /*
   * Periodic measurement every 7 seconds.
   * Interrupt enabled. No threshold set.
   */
  err = cotwo.startMeasure(7, 0, isr);
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("start measure error: ");
    Serial.println(err);
  }
}

void loop()
{
    /* 
     * Wait until a new CO2 meas sample
     * is available. When the isr is called
     * and the interrupt flag set.
    */
    while(false == intFlag) { };

    Serial.println("int occurred");
    intFlag = false;

    err = cotwo.getCO2(co2ppm);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("get co2 error: ");
      Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);
}