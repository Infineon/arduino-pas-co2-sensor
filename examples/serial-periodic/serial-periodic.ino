#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

#ifdef XMC1100_XMC2GO                                                   
uint8_t intPin = 9;
#else
uint8_t intPin = GPIOIno::unusedPin; /* Set the interrupt pin to the rigth value */
#endif

PASCO2SerialIno cotwo(&Wire, intPin);
int16_t co2ppm;
Error_t err;

volatile bool intFlag = false;
void isr (void * )
{
  intFlag = true;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("pas co2 serial initialized");

  err = cotwo.startMeasure(7, 0, isr);
  if(OK != err)
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
    if(OK != err)
    {
      Serial.print("get co2 error: ");
      Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);
}