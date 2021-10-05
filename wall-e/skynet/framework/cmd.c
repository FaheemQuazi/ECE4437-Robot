/*
 * cmd.c
 *
 *  Created on: Sep 30, 2021
 *      Author: faheem
 */

#include "cmd.h"


COMMAND CMD_defs[] = {
    { "ledr", &LED_ToggleR },
    { "ledg", &LED_ToggleG },
    { "ledb", &LED_ToggleB },
    { "dist", &Dist_Print },
    { "test", &CMD_DoNothing},
    { "mtm", &Motor_Forward },
    { "mts", &Motor_Stop },
    { "tr", &Motor_TurnR },
    { "tl", &Motor_TurnL },
    { "rev", &Motor_Reverse }
};


void CMD_DoNothing(UArg arg0, UArg arg1) {}

void tskCMDDispatcher(UArg arg0, UArg arg1) {
    const int s = sizeof(CMD_defs);
    int i = 0;
    char name[8];
    while (true) {
        Mailbox_pend(mbxCmd, &name, BIOS_WAIT_FOREVER);

        for (i = 0; i < s; i++) {
            COMMAND sc = CMD_defs[i];
            if (strcmp(sc.name, name) == 0) {
                // TODO: Dispatch command as task
                (*sc.fun_ptr)(NULL, NULL);
                break;
            }
        }

        if (i < s) {
            BT_PrintString("< ");
            BT_PrintString(name);
            BT_PrintString(" OK\r\n");
        } else {
            BT_PrintString("< ");
            BT_PrintString(name);
            BT_PrintString(" NC\r\n");
        }
    }
}
