#include "pid.h"

//****************Globals**************************
uint16_t PID_pulseWidth = 0;
uint16_t PID_convertedADCVal;
float PID_errorCurr = 0;
float PID_errorPrev = 0;
int16_t PID_totalSummation = 0;
int16_t PID_diff;


void PID_Init() {
    Motor_Forward(NULL, NULL);
    Motor_setspd_L(50);
}

void RunPIDController(){

    float P, I, D;

    PID_errorCurr = SETPOINT - Dist_GetRCM() + 4;


//    P = P_MULT * PID_errorCurr;
//
//    PID_totalSummation += PID_errorCurr;
//    I = I_MULT * PID_totalSummation; // Note, not using time as a simplification since it should be consistent
//
//    PID_diff = PID_errorCurr - PID_errorPrev;
//    PID_errorPrev = PID_errorCurr;
//    D = D_MULT * PID_diff;  // Note, not using time as a simplification since it should be consistent

//    PID_pulseWidth = (uint16_t)(P + I + D);

    Motor_setspd_R((uint16_t)PID_errorCurr * 10);



}
