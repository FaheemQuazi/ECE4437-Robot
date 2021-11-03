/*
 * cmd.h
 *
 *  Created on: Sep 30, 2021
 *      Author: faheem
 */

#ifndef SKYNET_FRAMEWORK_CMD_H_
#define SKYNET_FRAMEWORK_CMD_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

#include "skynet/drivers/led.h"
#include "skynet/drivers/bt.h"
#include "skynet/drivers/motor.h"
#include "skynet/drivers/lightsensor.h"
#include "skynet/drivers/dist.h"
#include "skynet/framework/control.h"

typedef struct {
    char name[8];
    void (*fun_ptr)(UArg, UArg);
} COMMAND;

void CMD_DoNothing(UArg, UArg);
void tskCMDDispatcher(UArg, UArg);


#endif /* SKYNET_FRAMEWORK_CMD_H_ */
