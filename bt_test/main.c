/*
 *  ======= main ========
 *
 *  Created on: Sep 1, 2021
 *  Author:     faheem
 *
 *  Pins:
 *  - PF1, PF2, PF3 : Onboard LEDs
 *  - PB3 : HC-05 STATE Pin
 *  - PE4, PE5 : HC-05 Tx/Rx (UART)
 */

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>                //needed for any Log_info() call
#include <xdc/cfg/global.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTTiva.h>


/*
 * ======== Hardware Configuration ========
 */

/* *** UART Configuration *** */
// From the uartecho Example
// use PE4 and PE5 (UART5)
UARTTiva_Object uartTivaObjects[1];
unsigned char uartTivaRingBuffer[1][32];
// UART configuration structures
const UARTTiva_HWAttrs uartTivaHWAttrs[1] = {
    {
        .baseAddr = UART5_BASE,
        .intNum = INT_UART5,
        .intPriority = (~0),
        .flowControl = UART_FLOWCONTROL_NONE,
        .ringBufPtr  = uartTivaRingBuffer[0],
        .ringBufSize = sizeof(uartTivaRingBuffer[0])
    }
};
const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTTiva_fxnTable,
        .object = &uartTivaObjects[0],
        .hwAttrs = &uartTivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};
// UART module handle
UART_Handle uart;

// *** Board Initialization Function ***
void Board_Init() {
    // ****** GPIO Init ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


    // ****** Interrupt Enable (see cfg for the hwi setup) ******
    // For BT STATE
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_3);

    // ****** UART Init ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UART_init();

    /* Configure the UART Module */
    UART_Params uartParams;
    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;
    uart = UART_open(0, &uartParams);

    if (uart == NULL) { // If UART fails
        // Turn on the RED LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
        System_abort("UART Fail");
        return;
    } else {
        // Turn on the GRN LED
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
    }
}

// ======== main ========
Int main(void)
{
    Board_Init();

    BIOS_start();
    return (0);
}

// ======== light flash clock routine ========
int lightCount = 0;
Void clkLight(UArg arg0) {
    if (lightCount++ > 50) {
        lightCount = 0;
        Semaphore_post(semPrintUart);
    }
}

// ======== hwi: bluetooth STATE pin ========
uint8_t BT_ENABLE = false;
void hwiBtEnable() {
    // Clear the Interrupt Status immediately
    uint32_t status = GPIOIntStatus(GPIO_PORTB_BASE, true);
    GPIOIntClear(GPIO_PORTB_BASE, status);

    // Post Semaphore to handle bluetooth status check
    Semaphore_post(semBTStatCheck);
}


// ======== task: UART Read ========
unsigned char rxBuffer[1];
int readSize;
void tskUartRead(UArg arg0, UArg arg1) {
    uint8_t pinstat = 0;
    while (true) {
        readSize = UART_read(uart, rxBuffer, sizeof(rxBuffer));
        UART_write(uart, rxBuffer, sizeof(rxBuffer));
        if (readSize > 0) {
            if (rxBuffer[0] == 'r') {
                pinstat ^= GPIO_PIN_1;
            } else if (rxBuffer[0] == 'g') {
                pinstat ^= GPIO_PIN_3;
            } else if (rxBuffer[0] == 'b') {
                pinstat ^= GPIO_PIN_2;
            }
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, pinstat);
            Semaphore_post(semPrintUart);
        }
    }
}

// ======== task: UART Write ========
char echoPrompt[32];
int echoPromptSize = 0;
void tskUartWrite(UArg arg0, UArg arg1) {
    while (true) {
        Semaphore_pend(semPrintUart, BIOS_WAIT_FOREVER); // wait for semaphore
        if (BT_ENABLE) {
            echoPromptSize = sprintf(echoPrompt, "WALL-E GO BRRRR %u\r\n", lightCount);
            UART_write(uart, echoPrompt, echoPromptSize);
        }
    }
}

// ======== task: BT State Check ========
void tskBTStatCheck(UArg arg0, UArg arg1) {
    while (true) {
        Semaphore_pend(semBTStatCheck, BIOS_WAIT_FOREVER); // wait for semaphore
        BT_ENABLE = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3) & GPIO_PIN_3;
    }
}
