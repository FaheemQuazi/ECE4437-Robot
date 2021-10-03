
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

void BT_PrintString(char str[]) {
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        UARTCharPut(UART5_BASE, str[i]);
    }
}

// ======== task: UART Read ========
void tskBTRead(UArg arg0, UArg arg1) {
    char rc[8];
    uint8_t p = 0;
    char r;

    rc[0] = '\0';
    UARTCharPut(UART5_BASE, '>');

    while(1) {

        // This line will wait until a character is received on the UART
        r = UARTCharGet(UART5_BASE);

        if (r == '\r') {
            if (strlen(rc) > 0) {
                BT_PrintString("\r\n");
                Mailbox_post(mbxCmd, &rc, BIOS_NO_WAIT);
                p = 0;
                rc[0] = '\0';
            }
            BT_PrintString("\r\n>");
        } else {
            switch (r) {
            case '\b':
                if (p > 0) {
                    p--;
                    rc[p] = '\0';
                    BT_PrintString("\b\e[0K"); // \b = backspace ; \e[0K clears the characters to the right of the cursor
                }
                break;
            default:
                if (p <= 6) {
                    // we have room for a char
                    rc[p] = r;
                    rc[++p] = '\0';
                    UARTCharPut(UART5_BASE, r);
                }
                break;
            }
        }

    }
}
