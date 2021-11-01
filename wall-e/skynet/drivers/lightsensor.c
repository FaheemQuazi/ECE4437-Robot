/*
 * lightsensor.c
 *
 *  Created on: Sep 29, 2021
 *      Author: faheem
 */

///------------------------------------------
// Light Sensor Demo
//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>

#include "inc/hw_timer.h"
#include "lightsensor.h"


void LightSensor_Init(void) {

       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Enable PortD Peripheral
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable PortF Peripheral
       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // Make PortF Pin1 Output
}


uint32_t startTime, endTime, pinValue;
int count = 0;
int state = 0;
void GetLight() {
    switch (state) {
    case 0:
        // set up light sensor
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);         // Make PortD pin1 output to charge the sensor
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);      // Start charing the sensor
        if (count < 2) {
            count++;
        } else {
            state = 1;
            count = 0;
        }
        break;
    case 1:
        GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);          // Make PortD pin1 input to time the decaying
        pinValue = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3);        // Assign the value when sensor is in fully charged state
        if (pinValue & GPIO_PIN_3) {
            count++;
        } else {
            state = 2;
        }
        break;
    case 2:
        if (count > 2) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        }
        count = 0;
        state = 3;
        break;
    case 3:
        if (count < 20) {
            count++;
        } else {
            count = 0;
            state = 0;
        }
        break;
    }

}

// delay()
// Creates a delay of i milliseconds via TivaWare fxn
//---------------------------------------------------------------------------
void delay(uint32_t i)
{
     SysCtlDelay(26800 * i);        // creates ~1ms delay - TivaWare fxn

}

