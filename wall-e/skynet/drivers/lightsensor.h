/*
 * lightsensor.h
 *
 *  Created on: Sep 29, 2021
 *      Author:
 */

#ifndef SKYNET_DRIVERS_LIGHTSENSOR_H_
#define SKYNET_DRIVERS_LIGHTSENSOR_H_

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>
#include "skynet/drivers/bt.h"
#include "skynet/drivers/motor.h"
#include "skynet/framework/control.h"
#include "skynet/drivers/dist.h"
#include "skynet/framework/pid.h"
#include "skynet/framework/cmd.h"
#include "skynet/framework/Timer.h"

#include "inc/hw_timer.h"

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


bool LS_sendData;

#endif /* SKYNET_DRIVERS_LIGHTSENSOR_H_ */
