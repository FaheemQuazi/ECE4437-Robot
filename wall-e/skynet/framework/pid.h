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

#define SETPOINT 6
#define P_MULT 10
#define I_MULT 5
#define D_MULT 5

void PID_Init();
void RunPIDController();




#endif /* SKYNET_FRAMEWORK_PID_H_ */
