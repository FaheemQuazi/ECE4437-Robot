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
#include <time.h>


#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/i2c.h"

#include "skynet/drivers/dist.h"
#include "skynet/drivers/motor.h"

#define PID_FWD_SPEED 50
#define PID_SETPOINT 2250
#define PID_P_MULT 0.025
#define PID_I_MULT 0
#define PID_D_MULT 0.02

void PID_Init();
void RunPIDController();




#endif /* SKYNET_FRAMEWORK_PID_H_ */
