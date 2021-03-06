#ifndef SKYNET_FRAMEWORK_PID_H_
#define SKYNET_FRAMEWORK_PID_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <driverlib/timer.h>
#include "skynet/drivers/dist.h"
#include "skynet/drivers/motor.h"
#include "skynet/framework/control.h"


#define PID_FWD_SPEED 100
#define PID_SETPOINT 2600
#define PID_P_MULT 0.05
#define PID_I_MULT 0
#define PID_D_MULT 0.015

int16_t pidval;
int16_t PID_errorCurr;
bool PID_Left;

void PID_Init();
void RunPIDController();
void swiPidMotor(UArg arg0, UArg arg1);

#endif /* SKYNET_FRAMEWORK_PID_H_ */
