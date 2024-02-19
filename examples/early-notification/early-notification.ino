#include <Arduino.h>
#include <pas-co2-ino.hpp>

/**
 * In this example, the interrupt is used to control the  12V emitter 
 * power supply, to just keep it enabled while the sensing is 
 * performed.
 * The interrupt signal will be enabled as long as the measure is 
 * being performed. 
 * At least, three options could be considered here:
 * 
 * 1. Direct hardware coupling of interrupt and 12V power supply.
 *    
 *    The interrupt signal is interfaced with a load control 
 *    circuitry which switches on/off the 12V signal. There is 
 *    no intervention of the controller. The interrupt signal
 *    is not be connected to the microcontroller. The controller
 *    can just synchronize the readout by delays and/or just poll 
 *    for the ready measurement.
 * 
 * 2. MCU coupling of interrupt and 12V power supply.
 * 
 *    The interrupt signal is interfaced with the microcontroller,
 *    which uses another digital output to enable or disable the 
 *    12V emitter power supply. 
 * 
 * 3. Both direct 12V coupling and MCU interfacing of interrupt signal.
 * 
 *    Alternatively, both options can be combined: the interrupt is 
 *    directly interfaced to the 12V power supply enablement
 *    signal, and it is additionally connected to the 
 *    microcontroller for only readout synchronization purposes.
 *
 *  In this example, we consider the third option. The micrcontroller
 *  waits for the falling edge on the interrupt signal to trigger the
 *  readout.
 */

uint8_t interruptPin = 9;      /* For XMC2Go. Change it for your hardware setup */

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ     400000  
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define EARLY_NOTIFICATION_ENABLED         true

/*
 * The constructor takes the Wire instance as i2c interface,
 * and the controller interrupt pin
 */
PASCO2Ino cotwo(&Wire, interruptPin);

int16_t co2ppm;
Error_t err;

/* 
 * A simple interrupt service routine. This callback will be 
 * executed every time that the sensor is about to start 
 * performing the measurement and when it is completed.
 * In the main loop we use that flag to synchronize the 
 * sensor CO2 concentration readout.
 */
volatile bool measurementReady = false;
void isr (void * )
{   
    uint8_t interruptPinLevel = digitalRead(interruptPin);
    /* A falling edge event*/
    if(interruptPinLevel == LOW)
    {
        measurementReady = true;
    }
    /* A rising edge event*/
    else if(interruptPinLevel == HIGH)
    {
        measurementReady = false;
    }
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
    * No need to initialize the interrupt pin. This is done 
    * in the sensor begin() function 
    */
    err = cotwo.begin();
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(err);
    }

    /*
    * Continuous measurement every 10 seconds.
    * Enable early notification enabled
    */
    err = cotwo.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS, 0, isr, EARLY_NOTIFICATION_ENABLED);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }
}

void loop()
{
    /* Wait for measurement to be ready */
    while(false == measurementReady) { };

    /* Clear the measurement ready flag */
    Serial.println("measurement ready");
    measurementReady = false;

    err = cotwo.getCO2(co2ppm);
    if(XENSIV_PASCO2_OK != err)
    {
      Serial.print("get co2 error: ");
      Serial.println(err);
    }

    Serial.print("co2 ppm value : ");
    Serial.println(co2ppm);
}