/*
 * Race Timer Functions
 */

#include "Timer.h"


void RaceTimer_Init() {
    timestarted = false;
    timesincestart = 0;
}

void GetTimer() {
    TimerIntClear(TIMER2_BASE, TimerIntStatus(TIMER2_BASE, false));

    // Tick timer if we're going now
    if (timestarted) {
       timesincestart = timesincestart + 1;
    }
}

//Reset and stop the timer
void resetTimer()
{
        timesincestart = 0;
        timestarted = false;
}



