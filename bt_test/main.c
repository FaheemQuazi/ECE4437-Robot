/*
 *  ======= main ========
 *
 *  Created on: Sep 27, 2021
 *  Author:     faheem
 *
 *  Pins:
 *  - PF1, PF2, PF3 : Onboard LEDs
 *  - PE4 : TX pin of HC-05 or BLUESMiRF
 *  - PE5 : RX pin of HC-05 or BLUESMiRF
 */

/* Standard C Libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants */
// This one gives all the GPIO_PORTx_BASE variables and the UARTx_BASE variables
#include <inc/hw_memmap.h>

/* DriverLib */
// All the SysCtl Functions
#include <driverlib/sysctl.h>
// Adds more constants like GPIO_PE4_U5RX and GPIO_PE4_U5TX for configuring pins
#include <driverlib/pin_map.h>
// GPIO Peripheral Driver Library
#include <driverlib/gpio.h>
// UART Peripheral Driver Library
#include <driverlib/uart.h>

// Set the baud rate with this variable (referenced below)
// HC-05: 9600 | BLUESMiRF: 115200
#define UART_BAUDRATE 9600

// *** Board Initialization Function ***
void Board_Init() {
    // ****** GPIO Initialization ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


    // ****** UART GPIO Pin Configuration ******
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);


    // ****** UART Module Configuration ******
    // See the peripheral driver documentation for how to use this function
    // UART_BAUDRATE is set in a #define above
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUDRATE,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
        UART_CONFIG_PAR_NONE));

    // Enable the UART Module
    UARTEnable(UART5_BASE);

    // Turn on the green light so we know it's running
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
}

// ======== main ========
int main(void)
{
    Board_Init();

    uint8_t pinstat = 0; // store the LED states in binary here
    char c; // store the char we receive from the UART here
    while(1) {

        // This line will wait until a character is received on the UART
        c = UARTCharGet(UART5_BASE);

        // send the character back over the UART for feedback
        UARTCharPut(UART5_BASE, c);

        // Toggle a byte in the pinstat variable based on which char was sent
        if (c == 'r') {
            pinstat ^= GPIO_PIN_1;
        } else if (c == 'g') {
            pinstat ^= GPIO_PIN_3;
        } else if (c == 'b') {
            pinstat ^= GPIO_PIN_2;
        }

        // Set all three GPIO LED pins on or off at the same time
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, pinstat);
    }
}
