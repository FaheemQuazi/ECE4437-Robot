//PWM Demo Code

//ECE 4437


//Include Headers

#include "motor.h"


void Motor_Init(){
    uint32_t val_load, pwm_clk;
    uint16_t duty = 70;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);


    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);


    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);

    pwm_clk = SysCtlClockGet() / 64;
    val_load = (pwm_clk / 100) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, val_load);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, val_load);

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, duty*val_load/100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, duty*val_load/100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, duty*val_load/100);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, duty*val_load/100);

    //PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_1);
    //PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);



        //Adjusting the pulse width based off the duty cycle value
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, duty*val_load/100);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, duty*val_load/100);
        //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, duty*val_load/100);
       // PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, duty*val_load/100);


}
void Motor_Forward(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);

    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

}
void Motor_Stop(UArg arg0, UArg arg1){
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
    //PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, false);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
}
void Motor_TurnR(UArg arg0, UArg arg1){
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

}
void Motor_TurnL(UArg arg0, UArg arg1){
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

}
void Motor_Reverse(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);



}


