/*
 * pid.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Caleb Ballard
 */

#ifndef SKYNET_FRAMEWORK_PID_H_
#define SKYNET_FRAMEWORK_PID_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "skynet/drivers/dist.h"
#include "skynet/drivers/motor.h"

#define PID_FWD_SPEED 90
#define PID_SETPOINT 2600
#define PID_P_MULT 0.025
#define PID_I_MULT 0
#define PID_D_MULT 0.015

void PID_Init();
void RunPIDController();
void swiLeftCheck();

#endif /* SKYNET_FRAMEWORK_PID_H_ */
