/*
 * motor.h
 *
 *  Created on: Sep 29, 2021
 *      Author: faheem
 */

#ifndef SKYNET_DRIVERS_MOTOR_H_
#define SKYNET_DRIVERS_MOTOR_H_

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
#include <driverlib/pwm.h>

#include "driverlib/uart.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

uint16_t Motor_Duty;
uint32_t Motor_Val_Load, Motor_pwm_clk;

void Motor_Init();
void Motor_Stop(UArg, UArg);
void Motor_Forward(UArg, UArg);
void Motor_TurnR(UArg, UArg);
void Motor_TurnL(UArg, UArg);
void Motor_Reverse(UArg, UArg);
void MotorSpdUp(UArg, UArg);
void MotorSpdDown(UArg, UArg);
void Motor_setspd_L(uint32_t);
void Motor_setspd_R(uint32_t);

#endif /* SKYNET_DRIVERS_MOTOR_H_ */
