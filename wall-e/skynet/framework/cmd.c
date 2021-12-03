#include "cmd.h"

COMMAND CMD_defs[] = {
    { "ledr", &LED_ToggleR },
    { "ledg", &LED_ToggleG },
    { "ledb", &LED_ToggleB },
    { "dist", &Dist_Print },
    { "test", &CMD_DoNothing},
    { "mtrg", &Motor_Stop },
    { "mtrs", &Motor_Start },
    { "ES", &setESTOP },
    { "EC", &clrESTOP },
    { "LC", &getLineCount },
    { "DD", &testStruct }
};

// Placeholder task to test the commanding system
void CMD_DoNothing(UArg arg0, UArg arg1) {}

// This task runs and dispatches tasks passed on by the mailbox
void tskCMDDispatcher(UArg arg0, UArg arg1) {
    const int s = sizeof(CMD_defs);
    int i = 0;
    MODBUS_PACKET recCmd;
    while (true) {
        Mailbox_pend(mbxCmd, &recCmd, BIOS_WAIT_FOREVER);
        for (i = 0; i < s; i++) {
            COMMAND sc = CMD_defs[i];
            if (strcmp(sc.name, recCmd.raw) == 0) {
                // TODO: Dispatch command as task
                (*sc.fun_ptr)(NULL, NULL);
                break;
            }
        }
    }
}
