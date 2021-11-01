/*
 * dist.h
 *
 *  Created on: Oct 1, 2021
 *      Author: faheem
 */

#ifndef SKYNET_DRIVERS_DIST_H_
#define SKYNET_DRIVERS_DIST_H_

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Constants */
#include <inc/hw_memmap.h>
#include "skynet/config.h"

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/pin_map.h>
#include "skynet/drivers/bt.h"

uint32_t DistF;
uint32_t DistR;

void Dist_Init();
void clkDistTrigger();
void swiDistTrigger();

uint16_t Dist_GetR();
uint16_t Dist_GetF();

void Dist_Print(UArg, UArg);



#endif /* SKYNET_DRIVERS_DIST_H_ */
