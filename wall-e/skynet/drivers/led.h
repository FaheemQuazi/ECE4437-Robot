#ifndef SKYNET_DRIVERS_LED_H_
#define SKYNET_DRIVERS_LED_H_

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
#include <inc/hw_memmap.h>
#include "skynet/config.h"

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>

uint8_t LED_pinstate;

void LED_Init();
void LED_ToggleR(UArg, UArg);
void LED_ToggleG(UArg, UArg);
void LED_ToggleB(UArg, UArg);

#endif /* SKYNET_DRIVERS_LED_H_ */
