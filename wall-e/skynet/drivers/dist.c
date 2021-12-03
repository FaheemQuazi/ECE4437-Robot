#include "dist.h"

void Dist_Init() {
    // Set Default Values
    DistF = 0;
    DistR = 0;

    // System Peripheral/GPIO Config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);

    // Front sensor
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntEnable(ADC0_BASE, 3);

    // Right sensor
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7);
    ADCSequenceEnable(ADC1_BASE, 3);
    ADCIntEnable(ADC1_BASE, 3);

}

// ==== SWI ====
// Stores the latest read ADC values for the distance sensors
void swiDistTrigger() {
    ADCSequenceDataGet(ADC0_BASE, 3, &DistF);
    ADCSequenceDataGet(ADC1_BASE, 3, &DistR);
}

// ==== HWI ====
// These are the ISRs for the ADC interrupts
void hwiDistF() {
    ADCIntClear(ADC0_BASE, 3);
    Swi_post(swiDist);
}
void hwiDistR() {
    ADCIntClear(ADC1_BASE, 3);
    Swi_post(swiDist);
}

// ==== Clock ====
// Trigger the ADC to take a reading
void clkDistTrigger() {
    ADCProcessorTrigger(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC1_BASE, 3);
}

// ==== Public Functions ====
uint16_t Dist_GetR() {
    return DistR;
}
uint16_t Dist_GetF() {
    return DistF;
}

// Helper/Test function used to print converted distance sensor values over BT/UART
void Dist_Print(UArg arg0, UArg arg1) {
    char *out = (char *)malloc(16*sizeof(char));

    float DR_CM = 4544.5 * pow((float)DistR,-0.834);

    float DF_CM = 10754 * pow((float)DistF,-0.953);

    sprintf(out, "%0.1f,%0.1f\r\n", DF_CM, DR_CM);
    BT_PrintString(out);
    free(out);
}

