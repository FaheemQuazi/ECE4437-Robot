#include "lightsensor.h"

MODBUS_PACKET dataPing;
MODBUS_PACKET dataPong;
MODBUS_PACKET* dataCurr;
MODBUS_PACKET* dataPrev;
int dataCount = 0;

char *LSRaceTime;

void LightSensor_Init(void) {
    dataPing.mb.colon = ':';
    dataPing.mb.addr = 1;
    dataPing.mb.chk = 1;
    dataPing.mb.cr = '\r';
    dataPing.mb.lf = '\n';
    dataPing.raw[25] = '\0';

    dataPong.mb.colon = ':';
    dataPong.mb.addr = 1;
    dataPong.mb.chk = 1;
    dataPong.mb.cr = '\r';
    dataPong.mb.lf = '\n';
    dataPong.raw[25] = '\0';

    dataCurr = &dataPing;

    LSRaceTime = (char*)malloc(sizeof(char) * 16);

   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                     // Enable PortD Peripheral
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                     // Enable PortF Peripheral
   GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);              // Make PortF Pin1 Output
}


uint32_t startTime, endTime, pinValue;
int count = 0;
int state = 0;
bool isBlack;

// Read the value for the light sensor - This is a clock function
void GetLight() {
    switch (state) {
    case 0:
        // set up light sensor
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);         // Make PortD pin 3 output to charge the sensor
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);      // Start charing the sensor
        if (count < 2) {                                            //threshold to start counting up
            count++;
        } else {
            state = 1;                                              //jump to case 1
            count = 0;                                              //reset counter
        }
        break;
    case 1:
        GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);          // Make PortD pin 3 input to time the decaying
        pinValue = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3);        // Assign the value when sensor is in fully charged state
        if (pinValue & GPIO_PIN_3) {
            count++;                                                //start counting
        } else {
            state = 2;                                              //jump to case 2
        }
        break;
    case 2:
        if (count > 2) {                                            //threshold to determine when on black surface
            isBlack = true;                                         //on black surface

        } else {
            isBlack = false;                                        //if less than threshold assume on white surface
        }
        count = 0;                                                  //reset the counter
        state = 3;                                                  //jump to case 3
        break;
    case 3:
        if (count < 20) {
            count++;                                                //continue counting up if threshold is less than 20
        } else {
            count = 0;                                              //if greater than 20 reset the counter
            state = 0;
        }
        break;
    }
    detectLine();                                                   //call function to determine the thickness of the line
}

int lineCount = 0;
bool onBlack = false;
//function to determine thickness of the black tape
void detectLine() {
    if (onBlack == false && isBlack == true) {                      //if previous state was white and current state is black
        onBlack = true;                                             //set previous state to black
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2, 0);
        lineCount = 1;                                              //start counting
    }

    if (onBlack == true && isBlack == true) {                       //if previous state was black and current state is also black
        lineCount++;                                                //keep counting
        if (lineCount > 20000) {
            lineCount = 20000;
        }
    }

    if (onBlack == true && isBlack == false) {                      //if previous state was black and current state is white
        onBlack = false;                                            //set previous state to white
        if (lineCount > 240) {                                      //if lineCount exceeds threshold for a single black line
            setESTOP();                                             //stop the robot
            timestarted = false;
            float timeinseconds = (float)timesincestart / 200;      //convert to seconds
            sprintf(LSRaceTime, "T%0.2fs\r\n", timeinseconds);      //print time in seconds from start until the robot stops at thick line
            BT_PrintString(LSRaceTime);
        } else if (lineCount > 90) {                                //if encounter thin black line send data
            if (LS_sendData) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);       //turn off led
                LS_sendData = false;
            } else {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
                LS_sendData = true;
            }
        }
    }
}

// Collect and handle MODBUS data
void tmrLSDataSender() {
    TimerIntClear(TIMER1_BASE, TimerIntStatus(TIMER1_BASE, false));
    if (ESTOP() || LS_sendData != true) { // Only send data if ESTOP is not set and we want to send data
        return;
    }

    int x = abs((float)PID_errorCurr / 2500 * 255); // Current Setpoint-Dist Error (Scaled to a 8-bit value)


    if (dataCount < 20) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        if (x > 255) x = 255;
        dataCurr->mb.data[dataCount] = (char) x;
        dataCount++;

    } else {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);

        // Swap buffers
        dataPrev = dataCurr;
        if (dataCurr == &dataPong) {
            dataCurr = &dataPing;
        } else {
            dataCurr = &dataPong;
        }
        dataCount = 0;

        // POST the Print Task
        Semaphore_post(semPrintDist);
    }
}

// Printing the MODBUS Data
void tskLSPrintData(UArg a0, UArg a1) {
    while (true) {
        Semaphore_pend(semPrintDist, BIOS_WAIT_FOREVER);
        BT_PrintString(dataPrev->raw);
    }
}

// Helper function to calibrate the light sensor
void getLineCount(UArg a0, UArg a1) {
    char *out = (char *)malloc(16*sizeof(char));

    sprintf(out, "%d\r\n", lineCount);
    BT_PrintString(out);
    free(out);
}

// Unused function to test printing the data structure
void testStruct(UArg a0, UArg a1) {
    BT_PrintString(dataPing.raw);
}

