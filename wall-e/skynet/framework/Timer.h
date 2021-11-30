/*
 * Timer.h
 *
 *  Created on: Nov 22, 2021
 *      Author: HP
 */

#ifndef SKYNET_FRAMEWORK_TIMER_H_
#define SKYNET_FRAMEWORK_TIMER_H_

#include <stdbool.h>
#include <xdc/runtime/System.h>
#include <inc/hw_memmap.h>

void GetTimer();
void RaceTimer_Init();
void resetTimer();

bool timestarted;
int timesincestart;

#endif /* SKYNET_FRAMEWORK_TIMER_H_ */
