#include "pid.h"

float PID_errorPrev = 0;
int16_t PID_totalSummation = 0;
int16_t PID_diff;
uint16_t PID_Fwd = PID_FWD_SPEED;
uint16_t speed;

void PID_Init() {
    Motor_Forward(NULL, NULL);
    Motor_setspd_L(PID_Fwd);
}

// Used to check the front sensor and turn left/u-turn
void FrontSensorAdjust() {
    uint16_t f = Dist_GetF();

    if (f > 2900) {
        PID_Left = true;
    }
}

// PID Control Algorithm
void swiPidMotor(UArg arg0, UArg arg1) {
    if (PID_Left) { // If we're turning left, don't do the PID algorithm
        uint16_t d = Dist_GetF();

        // Turn left until distance sensor sees opening
        Motor_setdir_L(false);
        Motor_setdir_R(true);
        Motor_setspd_L(PID_FWD_SPEED);
        Motor_setspd_R(PID_FWD_SPEED);

        if (d < 1000) {
            Motor_setdir_L(true);
            Motor_setdir_R(true);
            PID_Left = false;
        }

    } else { // Otherwise do the PID algorithm
        FrontSensorAdjust();

        // PID algorithm
        float P, I, D;

        PID_errorCurr = PID_SETPOINT - Dist_GetR();

        P = PID_P_MULT * PID_errorCurr;

        PID_totalSummation += PID_errorCurr;
        I = PID_I_MULT * PID_totalSummation; // Note, not using time as a simplification since it should be consistent

        PID_diff = PID_errorCurr - PID_errorPrev;
        PID_errorPrev = PID_errorCurr;
        D = PID_D_MULT * PID_diff;  // Note, not using time as a simplification since it should be consistent

        pidval = P + I + D;

        // Motor speed adjustment based on output of PID algorithm
        int16_t vr = PID_Fwd - pidval;
        int16_t vl = PID_Fwd - (vr - PID_Fwd);

        uint16_t speedr = (uint16_t)vr;
        uint16_t speedl = (uint16_t)vl;

        if (vr < 0){
            speedr = 1;
        }
        else if(vr > 100){
            speedr = 100;
        }

        if(vl < 0){
            speedl = 1;
        }
        else if(vl > 100){
            speedl = 100;
        }

        Motor_setspd_R(speedr);
        Motor_setspd_L(speedl);

    }
}

// Timer ISR to trigger the PID algorithm
void RunPIDController(){
    TimerIntClear(TIMER0_BASE, TimerIntStatus(TIMER0_BASE, false));
    if (ESTOP()) {
        Motor_Stop(NULL, NULL);
        return;
    } else {
        Motor_Start(NULL, NULL);
    }

    Swi_post(swiPid);
}
