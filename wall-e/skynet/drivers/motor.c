//PWM Demo Code

//ECE 4437


//Include Headers

#include "motor.h"

void Motor_Init(){
    Motor_Duty = 10;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);


    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);


    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PC5_M0PWM7);
    GPIOPinConfigure(GPIO_PC4_M0PWM6);

    Motor_pwm_clk = SysCtlClockGet() / 64;
    Motor_Val_Load = (Motor_pwm_clk / 100) - 1;
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, Motor_Val_Load);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, Motor_Duty*Motor_Val_Load/100);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, Motor_Duty*Motor_Val_Load/100);

    PWMGenEnable(PWM0_BASE, PWM_GEN_3);

}

void Motor_Forward(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);

    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

}

void Motor_Stop(UArg arg0, UArg arg1){

    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
}

void Motor_TurnR(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

}

void Motor_TurnL(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);

    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

}

void Motor_Reverse(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);



}

void Motor_setspd_L(uint32_t Motor_DutyL){
    if (Motor_DutyL > 90)  Motor_DutyL = 90;
    if (Motor_DutyL & 0x80000000 == 0x80000000) Motor_DutyL = 0;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, Motor_DutyL*Motor_Val_Load/100);
}

void Motor_setspd_R(uint32_t Motor_DutyR){
    if (Motor_DutyR > 90)  Motor_DutyR = 90;
    if (Motor_DutyR & 0x80000000 == 0x80000000) Motor_DutyR = 0;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, Motor_DutyR*Motor_Val_Load/100);
}

void MotorSpdUp(UArg arg0, UArg arg1){
    if(Motor_Duty < 90){
        Motor_Duty = Motor_Duty + 10;
        Motor_setspd_R(Motor_Duty);
        Motor_setspd_L(Motor_Duty);
    }

}

void MotorSpdDown(UArg arg0, UArg arg1){

    if(Motor_Duty > 10){
        Motor_Duty = Motor_Duty - 10;
        Motor_setspd_R(Motor_Duty);
        Motor_setspd_L(Motor_Duty);
    }

}
