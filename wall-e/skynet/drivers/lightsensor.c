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
#include "skynet/drivers/bt.h"
#include "skynet/drivers/motor.h"
#include "skynet/framework/control.h"
#include "skynet/drivers/dist.h"
#include "skynet/framework/pid.h"

#include "inc/hw_timer.h"
#include "lightsensor.h"

typedef struct {
    char colon;
    char addr;
    char func;
    char data[20];
    char chk;
    char cr;
    char lf;
} MODBUS_DATA;

union {
    char raw[26];
    MODBUS_DATA mb;
} d;


void LightSensor_Init(void) {
        d.mb.colon = ':';
        d.mb.addr = 1;
        d.mb.func = 'L';

        d.mb.chk = 'Z';
        d.mb.cr = '\r';
        d.mb.lf = '\n';

       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Enable PortD Peripheral
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable PortF Peripheral
       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // Make PortF Pin1 Output
}


uint32_t startTime, endTime, pinValue;
int count = 0;
int state = 0;
bool isBlack;

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
            isBlack = true;

        } else {
            isBlack = false;
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
    detectLine();
}

int lineCount = 0;
bool onBlack = false;
bool sendData = false;
void detectLine() {
    if (onBlack == false && isBlack == true) {          //are we over black or white?
        onBlack = true;                                 //set onBlack = true
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 0);
        lineCount = 1;                                //start counting
    }

    if (onBlack == true && isBlack == true) {
        lineCount++;                               //keep counting
        if (lineCount > 20000) {
            lineCount = 20000;
        }
    }

    if (onBlack == true && isBlack == false) {
        onBlack = false;
        if (lineCount > 240) {
            setESTOP();
        } else if (lineCount > 90) {
            sendData = !sendData;
            if (sendData) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
            } else {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            }
        }
    }
}

void clkDataSender() {
    if (sendData & !ESTOP()) {
        Semaphore_post(semPrintDist);
    }

    if (ESTOP()) {
        sendData = false;
    }
}


void tskLSDataSender(UArg arg0, UArg arg1) {
    int i = 0;
    while (true) {
        Semaphore_pend(semPrintDist, BIOS_WAIT_FOREVER);
        if (i < 20) {
            if (pidval > 127) {
                d.mb.data[i] = 255;
            } else if (pidval < -127) {
                d.mb.data[i] = 0;
            } else {
                d.mb.data[i] = (char) pidval;
            }
            i++;

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        } else {
            if (!PID_Left) {
                BT_PrintString("2S\r\n");
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
                i = 0;
            }
        }
    }
}

void getLineCount(UArg a0, UArg a1) {
    char *out = (char *)malloc(16*sizeof(char));

    sprintf(out, "%d\r\n", lineCount);
    BT_PrintString(out);
    free(out);
}

void testStruct(UArg a0, UArg a1) {
    BT_PrintString(d.raw);
}

