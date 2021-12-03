#include "motor.h" // Includes the motor header file.

void Motor_Init(){  // Initializes both motors.

    Motor_Duty = 10;                                        // Default motors duty speed of 10% out of 100%

//********************SysCtlPeripheralEnable**************
//  SYSCTL_PERIPH_PWM1 and SYSCTL_PERIPH_PWM0 are the peripherals to enable.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);             // Enables PWM1 peripheral.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);             // Enables PWM0 Peripheral.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);            // Enables Port C Peripheral.
//********************SysCtlPWMClockSet*******************
//  SYSCTL_PWMDIV_64 is the configuration for the PWM clock.
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);                    // Sets the PWM clock configuration.
//********************GPIOPinTypeGPIOOutput***************
//  GPIO_PORTC_BASE is the base address of the GPIO port.
//  GPIO_PIN_7 and GPIO_PIN_6 are the bit-packed representation of the pins.
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);     // Configures Port C pin 7 for use as a GPIO output.
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);     // Configures Port C pin 6 for use as a GPIO output.
//********************GPIOPinWrite************************
//  GPIO_PORTC_BASE is the base address of the GPIO port.
//  GPIO_PIN_7 and GPIO_PIN_6 are the bit-packed representation of the pins.
//  0 is the value to write to the pin.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);           // Writes a value to Port C pin 7.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);           // Writes a value to Port C pin 6.
//********************GPIOPinTypePWM**********************
// GPIO_PORTC_BASE is the base address of the GPIO port.
// GPIO_PIN_5 and GPIO_PIN_4 are the bit-packed representation of the pins.
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);            // Configures Port C pin 5 for use by the PWM peripheral.
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);            // Configures Port C pin 5 for use by the PWM peripheral.
//********************GPIOPinConfigure********************
// GPIO_PC5_M0PWM7 and GPIO_PC4_M0PWM6 are the pin configuration values.
    GPIOPinConfigure(GPIO_PC5_M0PWM7);                      // Configures the alternate function of a GPIO Port C pin 5.
    GPIOPinConfigure(GPIO_PC4_M0PWM6);                      // Configures the alternate function of a GPIO Port C pin 4.
//********************************************************

    Motor_pwm_clk = SysCtlClockGet() / 64;                  // A fraction of the PWM clock.
    Motor_Val_Load = (Motor_pwm_clk / 100) - 1;             // Gets the period of PWM generator, measured in clock ticks.

//********************PWMGenConfigure*********************
//  PWM0_BASE is the base address of the PWM module.
//  PWM_GEN_3 is the PWM generator to configure.
//  PWM_GEN_MODE_DOWN is the configuration for the PWM generator.
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN); //Configures a PWM generator.
//********************PWMGenPeriodSet*********************
// PWM0_BASE is the base address of the PWM module.
// PWM_GEN_3 is the PWM generator to be modified.
// Motor_Val_Load specifies the period of PWM generator output, measured in clock ticks.
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, Motor_Val_Load);  // Sets the period of a PWM generator.
//********************PWMPulseWidthSet********************
// PWM0_BASE is the base address of the PWM module.
// PWM_OUT_6 and PWM_OUT_6 are the PWM output to modify.
// Motor_Duty*Motor_Val_Load/100 specifies the width of the positive portion of the pulse.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, Motor_Duty*Motor_Val_Load/100); // Sets the pulse width for the PWM 6 output.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, Motor_Duty*Motor_Val_Load/100); // Sets the pulse width for the PWM 7 output.
//********************PWMGenEnable***********************
// PWM0_BASE is the base address of the PWM module.
// PWM_GEN_3 is the PWM generator to be enabled.
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);                     // Enables the timer/counter for a PWM generator block.
//********************************************************
}

//********************Motor_Forward***********************
// This function enables both motors to move in a forward direction.
void Motor_Forward(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);           // Writes a value to Port C pin 6.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);           // Writes a value to Port C pin 7.

    Motor_Start(UArg arg0, UArg arg1);                      // This function enables both motors to move.
}
//********************Motor_Start*************************
// This function enables both motors to move.
void Motor_Start(UArg arg0, UArg arg1) {
    //********************PWMOutputState******************
    // PWM0_BASE is the base address of the PWM module.
    // PWM_OUT_6_BIT and PWM_OUT_7_BIT are are the PWM outputs to be modified.
    // True enables the signal.
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);         // Enables PWM 6 output.
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);         // Enables PWM 7 output.
}
//********************Motor_Stop*************************
// This function disables both motors.
void Motor_Stop(UArg arg0, UArg arg1){

    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);         // Disables PWM 6 output.
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);         // Disables PWM 7 output.
}
//********************Motor_TurnR************************
// This function set the motors in a way that makes the robot turn right.
void Motor_TurnR(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);  // Reverses the direction of the right motor.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);           // Sets the left motor in a forward direction.

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);         // Enables the right motor.
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);         // Enables the left motor.

}
//********************Motor_TurnL************************
// This function set the motors in a way that makes the robot turn left.
void Motor_TurnL(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);           // Sets the right motor in a forward direction.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);  // Reverses the direction of the left motor.

    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);         // Enables the right motor.
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);         // Enables the left motor.

}
//********************Motor_Reverse************************
// This function enables both motors to move in a reversed direction.
void Motor_Reverse(UArg arg0, UArg arg1){

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);  // Reverses the direction of the right motor.
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);  // Reverses the direction of the left motor.

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);         // Enables the right motor.
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);         // Enables the left motor.



}
//********************Motor_setspd_L***********************
// Sets the speed of the left motor.
void Motor_setspd_L(uint32_t Motor_DutyL){
    if (Motor_DutyL > 100)  Motor_DutyL = 100;              // Keeps the motor speed within a 100 cap limit.
    if ((int32_t)Motor_DutyL < 0) Motor_DutyL = 0;          // Keeps the motor speed to a minimal value limit of 0.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, Motor_DutyL*Motor_Val_Load/100); // Sets the pulse width for the PWM 7 output.
}
//********************Motor_setspd_R***********************
// Sets the speed of the right motor.
void Motor_setspd_R(uint32_t Motor_DutyR){
    if (Motor_DutyR > 100) Motor_DutyR = 100;              // Keeps the motor speed within a 100 cap limit.
    if ((int32_t)Motor_DutyR < 0) Motor_DutyR = 0;         // Keeps the motor speed to a minimal value limit of 0.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, Motor_DutyR*Motor_Val_Load/100); // Sets the pulse width for the PWM 6 output.
}
//********************Motor_setdir_R***********************
// Sets the direction of the right motor.
void Motor_setdir_R(Bool d) { // false is reverse
    if (d) {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0); // Sets the right motor in a forward direction.
    } else {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7); // Reverses the direction of the right motor.
    }
}
//********************Motor_setdir_L***********************
// Sets the direction of the left motor.
void Motor_setdir_L(Bool d) { // false is reverse
    if (d) {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0); // Sets the left motor in a forward direction.
    } else {
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6); // Reverses the direction of the left motor.
    }
}
//********************MotorSpdUp**************************
// Increases the speed of both motors.
void MotorSpdUp(UArg arg0, UArg arg1){
    if(Motor_Duty < 90){
        Motor_Duty = Motor_Duty + 10; // Increases the motor speed by a factor of 10 once the function is called.
        Motor_setspd_R(Motor_Duty);   // Sends new motor speed to the right motor.
        Motor_setspd_L(Motor_Duty);   // Sends new motor speed to the left motor.
    }

}
//********************MotorSpdDown************************
// Decreases the speed of both motors.
void MotorSpdDown(UArg arg0, UArg arg1){

    if(Motor_Duty > 10){
        Motor_Duty = Motor_Duty - 10; // Decreases the motor speed by a factor of 10 once the function is called.
        Motor_setspd_R(Motor_Duty);   // Sends new motor speed to the right motor.
        Motor_setspd_L(Motor_Duty);   // Sends new motor speed to the left motor.
    }

}
