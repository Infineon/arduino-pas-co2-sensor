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

Diag_t diagnosis;

void setup()
{
  Serial.begin(9600);
  Serial.println("pas co2 serial initialized");
  
  Error_t err = cotwo.getDiagnosis(diagnosis);
  if(pasco2::OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.println("Diagnosis flags (1) true, (0) false : \n");
  Serial.print("sensor ready              : ");
  Serial.println(diagnosis.sensorRdy);
  Serial.print("pwm pin enabled           : ");
  Serial.println(diagnosis.pwmPinEnabled);
  Serial.print("temperature ouf of range  : ");
  Serial.println(diagnosis.outOfRangeTempError);
  Serial.print("ir emitter v out of range : ");
  Serial.println(diagnosis.outOfRange12VError);
  Serial.print("communication error       : ");
  Serial.println(diagnosis.commError);

}

void loop()
{

}