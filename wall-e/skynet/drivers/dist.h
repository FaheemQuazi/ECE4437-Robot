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

/* Constants */
#include <inc/hw_memmap.h>
#include "skynet/config.h"

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/adc.h>
#include <driverlib/pin_map.h>

uint32_t DistF;
uint32_t DistR;

void Dist_Init();
void Dist_GetValue();


#endif /* SKYNET_DRIVERS_DIST_H_ */
