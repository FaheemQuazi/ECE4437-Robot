/*
 * Timer.c
 *
 *  Created on: Nov 22, 2021
 *      Author: HP
 */

#include "Timer.h"


void RaceTimer_Init() {
    timestarted = false;
    timesincestart = 0;
}

void GetTimer() {
    //start timer
    if (timestarted) {
       timesincestart = timesincestart + 1;
    }
}

void resetTimer()
{
        timesincestart = 0;
}



