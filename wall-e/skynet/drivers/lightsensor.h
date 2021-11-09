/*
 * lightsensor.h
 *
 *  Created on: Sep 29, 2021
 *      Author: faheem
 */

#ifndef SKYNET_DRIVERS_LIGHTSENSOR_H_
#define SKYNET_DRIVERS_LIGHTSENSOR_H_

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
#include <inc/hw_memmap.h>
#include "skynet/config.h"

/* XDC */
#include <xdc/runtime/System.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>

void LightSensor_Init();
void GetLight();
void delay();
void detectLine();
void getLineCount(UArg, UArg);
void tskLSDataSender(UArg, UArg);
void testStruct(UArg, UArg);

#endif /* SKYNET_DRIVERS_LIGHTSENSOR_H_ */
