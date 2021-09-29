
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

// ======== task: UART Read ========
void tskBTRead(UArg arg0, UArg arg1) {
    uint8_t pinstate = 0; // store the LED states in binary here
    char c; // store the char we receive from the UART here
    while(1) {

        // This line will wait until a character is received on the UART
        c = UARTCharGet(UART5_BASE);

        // send the character back over the UART for feedback
        UARTCharPut(UART5_BASE, c);

        // Toggle a byte in the pinstate variable based on which char was sent
        if (c == 'r') {
            pinstate ^= GPIO_PIN_1;
        } else if (c == 'g') {
            pinstate ^= GPIO_PIN_3;
        } else if (c == 'b') {
            pinstate ^= GPIO_PIN_2;
        }

        // Set all three GPIO LED pins on or off at the same time
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, pinstate);
    }
}
