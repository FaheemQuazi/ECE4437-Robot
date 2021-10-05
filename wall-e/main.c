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
 *  - PF2: PWM6    -> A-Enable
 *  - PF3: PWM7    -> B-Enable
 *  - PC7: GPIO DO -> B-Phase
 *  - PC6: GPIO DO -> A-Phase
 *
 */

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>

/* Our code */
#include "skynet/drivers/led.h"
#include "skynet/drivers/bt.h"
#include "skynet/drivers/motor.h"
#include "skynet/drivers/lightsensor.h"
#include "skynet/framework/cmd.h"


// *** Board Initialization Function ***
void Board_Init() {
    LED_Init();
    BT_Init();
    Motor_Init();
    Dist_Init();
}

// *** Main ***
int main(void)
{
    Board_Init();
    BIOS_start();
    return (0);
}
