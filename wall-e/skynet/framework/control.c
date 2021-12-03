#include "control.h"

bool estop = true;

bool ESTOP() {
    return estop;
}

void setESTOP() {
    estop = true;
}

void clrESTOP() {
    estop = false;
}
