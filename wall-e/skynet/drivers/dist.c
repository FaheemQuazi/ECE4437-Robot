/*
 * dist.c
 *
 *  Created on: Oct 1, 2021
 *      Author: faheem
 */

#include "dist.h"


void Dist_Init() {
    // Front sensor
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
                             ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE, 0);

}

void Dist_GetValue() {

}
