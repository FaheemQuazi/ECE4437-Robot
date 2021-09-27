/*
 *  ======= main ========
 *
 *  Created on: Sep 16, 2021
 *  Author:     Team 1
 *  Members:    Faheem Quazi, Aruba Bukhari, Caleb Ballard
 *
 *  Pin Defs:
 *  - PB3: GPIO DO -> BT STATE
 *  - PE4: UART RX -> BT TX
 *  - PE5: UART TX -> BT RX
 *  - PD0: AIN7    -> Dist R
 *  - PD1: AIN6    -> Dist F
 *
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
#include <xdc/runtime/Log.h>
#include <xdc/cfg/global.h>
/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/uart.h>

/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTTiva.h>

// *** Board Initialization Function ***
void Board_Init() {
    // ****** GPIO Init ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

/*
 * ======== main ========
 */
Int main(void)
{
    Board_Init();
    BIOS_start();
    return (0);
}

// ======== blink LED clock routine ========
int lightCount = 0;
Void clkLight(UArg arg0) {
    if (lightCount % 2 == 0) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14);
    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    if (lightCount++ > 10) {
        lightCount = 0;
    }
}

/*
 * ======== Hardware Configuration ========
 */

/* *** UART Configuration *** */
// use PE4 and PE5 (UARTS)
UARTTiva_Object uartTivaObjects[1];
unsigned char uartTivaRingBuffer[1][32];
// UART configuration structures
const UARTTiva_HWAttrs uartTivaHWAttrs[1] = {
    {
          .baseAddr = UARTS_BASE,
          .intNum = INT_UARTS,
          .intPriority = (~0),
          .flowControl = UART_FLOWCONTROL_NONE,
          .ringBufPtr = uartTivaRingBuffer[0],
          .ringBufSize = sizeof(uartTivaRingBuffer[0])
    }
};

const UART_Config UART_config[] = {
    {
         .fxnTablePtr = &UARTTiva_fxnTable,
         .object = &uartTivaObjects[0],
         .hwAttrs = &uartTivaHWAttrs
    },
    {NULL, NULL, NULL}
};
// UART module handle
UART_Handle uart;

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
uart.readDataMode = UART_DATA_BINARY;
uartParams.readReturnMode = UART_RETURN_FULL;
uartParams.readEcho = UART_ECHO_OFF;
uartParams.baudRate = 9600;
uart = UART_open(0, &uartParams);

if(uart == NULL){
    // If UART fails
    // Turn on the RED LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
    System_abort("UART Fail");
    return;
} else {
    // Turn on the GREEN LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
}

// ======== task: UART Write ========
char echoPrompt[32];
void tskUartWrite(UArg arg0, UArg arg1) {
    while (true) {
        Semaphore_pend(semPrintUart, BIOS_WAIT_FOREVER); // wait for semaphore
        if (BT_ENABLE) {
            sprintf(echoPrompt, "WALL-E SAYS HELLO %u\r\n", lightCount);
            UART_write(uart, echoPrompt, sizeof(echoPrompt));
        }
    }
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
            } else if  (rxBuffer[0] == 'b') {
                pinstat ^= GPIO_PIN_2;
            }
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, pinstat);
            Semaphore_post(semPrintUart);
        }
    }
}



