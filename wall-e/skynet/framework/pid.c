#include "pid.h"

//****************Globals**************************
float PID_errorPrev = 0;
int16_t PID_totalSummation = 0;
int16_t PID_diff;


void PID_Init() {
    Motor_Forward(NULL, NULL);
    Motor_setspd_L(PID_FWD_SPEED);
}

void RunPIDController(){
    float P, I, D;

    int16_t PID_errorCurr = PID_SETPOINT - Dist_GetR();

    P = PID_P_MULT * PID_errorCurr;

    PID_totalSummation += PID_errorCurr;
    I = PID_I_MULT * PID_totalSummation; // Note, not using time as a simplification since it should be consistent

    PID_diff = PID_errorCurr - PID_errorPrev;
    PID_errorPrev = PID_errorCurr;
    D = PID_D_MULT * PID_diff;  // Note, not using time as a simplification since it should be consistent

    int16_t v = PID_FWD_SPEED - (P + I + D);

    uint16_t speed = (uint16_t)v;
    if (v < 0) speed = 0;
    if (v > 100) speed = 100;
    Motor_setspd_R(speed);

    return;
}