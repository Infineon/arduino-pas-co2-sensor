#include <Arduino.h>
#include <pas-co2-serial-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ     400000  

uint8_t interrupt_pin = 9;      /* For XMC2Go. Change it for your hardware setup */

/*
 * The constructor takes the Wire instance as i2c interface,
 * and the controller interrupt pin
 */
PASCO2SerialIno cotwo(&Wire, interrupt_pin);

int16_t co2ppm;
Error_t err;

/* 
 * A simple interrupt service routine. This callback will be 
 * executed every time that the sensor alarm is triggered,
 * and set the flag to true. In the main loop we use that
 * flag to synchronize the co2 read out when a new value is
 * available.
 */
volatile bool intFlag = false;
void isr (void * )
{
  intFlag = true;
}

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("serial initialized");

  /* Initialize the i2c serial interface used by the sensor */
  Wire.begin();
  Wire.setClock(I2C_FREQ_HZ);

  /*
   * No need to initialized the interrupt pin. This is done 
   * in the sensor begin() function 
   */
  err = cotwo.begin();
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(err);
  }

  /*
   * Periodic measurement every 10 seconds.
   * Interrupt alarm when the CO2 value 
   * goes over 25000 ppm. The isr function is 
   * passed enabling the sensor interrupt mode.
   */
  err = cotwo.startMeasure(10, 1200, isr);
  if(XENSIV_PASCO2_OK != err)
  {
    Serial.print("start measure error: ");
    Serial.println(err);
  }
}

void loop()
{
    /* Interrupt alarm requires a CO2 concentration above the threshold */
    Serial.println("USER ACTION REQUIRED --> increase co2 to 1200 PPM to trigger the alarm!!");
    while(false == intFlag) { };

    /* Clear the interrupt flag */
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