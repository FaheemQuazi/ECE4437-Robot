/*
 * led.c
 *
 *  Created on: Sep 30, 2021
 *      Author: faheem
 */

#include "led.h"

void LED_Init()  {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

void LED_ToggleR(UArg arg0, UArg arg1) {
    LED_pinstate ^= GPIO_PIN_1;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, LED_pinstate);
}
void LED_ToggleG(UArg arg0, UArg arg1) {
    LED_pinstate ^= GPIO_PIN_3;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, LED_pinstate);
}
void LED_ToggleB(UArg arg0, UArg arg1) {
    LED_pinstate ^= GPIO_PIN_2;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, LED_pinstate);
}
