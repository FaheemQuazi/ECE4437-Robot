
#include "bt.h"


void BT_Init() {
    // ****** UART GPIO Pin Configuration ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // ****** UART Module Configuration ******
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUDRATE,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
        UART_CONFIG_PAR_NONE));

    // Enable the UART Module
    UARTEnable(UART5_BASE);
}

// Loop through a string and print the characters to the UART
void BT_PrintString(char str[]) {
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        UARTCharPut(UART5_BASE, str[i]);
    }
}

// ======== task: UART Read ========
void tskBTRead(UArg arg0, UArg arg1) {
    // Commented out code in this task is for a proper commanding setup
    // We did not use it for the competition because it was taking up too much resources
    // which were not necessary for the competition


//    MODBUS_PACKET rc;
//    uint8_t p = 0;
//    rc.raw[0] = '\0';
    char r;

    BT_PrintString("OK");
    while(1) {

        // This line will wait until a character is received on the UART
        r = UARTCharGet(UART5_BASE);

        if (r == 'e') {
           setESTOP(); // emergency stop (or halt almost all functions)
        } else if (r == 'c') {
            clrESTOP(); // clear ESTOP (or resume/start all functions)
            timestarted = true; // start the race timer
        } else if (r == 'r') {
            resetTimer(); // reset the race timer
        }


//        if (r == '\r') {
//            if (strlen(rc.raw) > 0) {
//                Mailbox_post(mbxCmd, &rc, BIOS_NO_WAIT);
//                p = 0;
//            }
//        } else {
//            rc.raw[p++] = r;
//        }

    }
}
