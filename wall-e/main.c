/*
 *  ======= main ========
 *
 *  Created on: Sep 16, 2021
 *  Author:     Team 1
 *  Members:    Faheem Quazi, Aruba Bukhari, Caleb Ballard
 *
 *  Pin Defs:
 *  - PB3: GPIO DO -> BT STATE
 *  - PE4: UART RX -> BT TX
 *  - PE5: UART TX -> BT RX
 *  - PD0: AIN7    -> Dist R
 *  - PD1: AIN6    -> Dist F
 *
 */

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
#include <inc/hw_memmap.h>
#include "skynet/config.h"

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>

#include "skynet/drivers/bt.h"

// *** Board Initialization Function ***
void Board_Init() {
    // ****** GPIO Init ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    BT_Init();
}

/*
 * ======== main ========
 */
int main(void)
{
    Board_Init();
    BIOS_start();
    return (0);
}

// ======== blink LED clock routine ========
int lightCount = 0;
Void clkLight(UArg arg0) {
    if (lightCount % 2 == 0) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14);
    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    if (lightCount++ > 10) {
        lightCount = 0;
    }
}



