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
    //Start timer if time has started
    if (timestarted) {
       timesincestart = timesincestart + 1;
    }
}

    //Reset the timer
void resetTimer()
{
        timesincestart = 0;
        timestarted = false;
}



