#include "pid.h"










//****************Globals**************************
    uint16_t PID_pulseWidth = 0;
    uint16_t PID_convertedADCVal;
    int16_t PID_errorCurr;
    int16_t PID_errorPrev = 0;
    int16_t PID_totalSummation = 0;
    int16_t PID_diff;




void RunPIDController(void){

        float P, I, D;

        PID_errorCurr = SETPOINT - (uint16_t)(Dist_GetRCM());


        P = P_MULT * PID_errorCurr;

        PID_totalSummation += PID_errorCurr;
        I = I_MULT * PID_totalSummation; // Note, not using time as a simplification since it should be consistent

        PID_diff = PID_errorCurr - PID_errorPrev;
        PID_errorPrev = PID_errorCurr;
        D = D_MULT * PID_diff;  // Note, not using time as a simplification since it should be consistent

        PID_pulseWidth = (uint16_t)(P + I + D);

        Motor_setspd_R(PID_pulseWidth);



}
