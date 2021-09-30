/*
 * cmd.c
 *
 *  Created on: Sep 30, 2021
 *      Author: faheem
 */

#include "cmd.h"

COMMAND CMD_defs[] = {
    { "ledr", 5, &LED_ToggleR },
    { "ledg", 5, &LED_ToggleG },
    { "ledb", 5, &LED_ToggleB },
    { "test", 3, &CMD_DoNothing}
};


void CMD_DoNothing(UArg arg0, UArg arg1) {}

bool CMD_Dispatch(char name[]) {
    int i = 0;
    for (i = 0; i < sizeof(CMD_defs); i++) {
        COMMAND sc = CMD_defs[i];
        if (strcmp(sc.name, name) == 0) {
            Task_Handle tskCmdH;
            Task_Params tskCmdP;

            Task_Params_init(&tskCmdP);
            tskCmdP.priority = sc.prio;

            tskCmdH = Task_create(sc.fun_ptr, &tskCmdP, NULL);

            Task_delete(&tskCmdH);
            return true;
        }
    }
    return false;
}
