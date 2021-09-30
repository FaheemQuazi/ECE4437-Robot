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
#include <ti/sysbios/knl/Task.h>

#include "skynet/drivers/led.h"
#include "skynet/drivers/bt.h"
#include "skynet/drivers/motor.h"
#include "skynet/drivers/lightsensor.h"

typedef struct {
    char name[8];
    uint8_t prio;
    void (*fun_ptr)(UArg, UArg);
} COMMAND;

void CMD_DoNothing(UArg, UArg);
bool CMD_Dispatch(char[]);


#endif /* SKYNET_FRAMEWORK_CMD_H_ */
