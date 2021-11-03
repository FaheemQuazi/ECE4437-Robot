/*
 * control.c
 *
 *  Created on: Nov 3, 2021
 *      Author: faheem
 */


#include "control.h"

bool estop;

bool ESTOP() {
    return estop;
}

void setESTOP() {
    estop = true;
}

void clrESTOP() {
    estop = false;
}
