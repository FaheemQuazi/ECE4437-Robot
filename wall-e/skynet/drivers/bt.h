

#ifndef SKYNET_DRIVERS_BT_H_
#define SKYNET_DRIVERS_BT_H_

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
#include <driverlib/uart.h>


void BT_Init();

void tskBTRead(UArg arg0, UArg arg1);


#endif /* SKYNET_DRIVERS_BT_H_ */
