/*
Author Name: Serak Gebremedhin
Midterm 1 Code
Date: 10/31/2019



*/


/*******************************************************

Goal: The goal of task 1 ant two is to interface the given MPU6050 IMU using
I2C protocol to TIVAC. The values will be printed on the serial terminal,
and they can also be printed using the graph on code composer studio.

Steps: The raw values will pass the the complementary filter function, and
will print on the serial terminal. To implement the filter, the IQMath library
will be used.

******************************************************
Chip type               : ARM TM4C123GH6PM
Program type            : Firmware
Core Clock frequency    : 80.000000 MHz
*******************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "utils/uartstdio.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "sensorlib/hw_mpu6050.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/i2cm_drv.c"
#include "sensorlib/mpu6050.h"
#include "utils/uartstdio.h"

#define ACCEL_SLAVE_ADDR 0x1D
#define XOUT8 0x06
#define YOUT8 0x07
#define ZOUT8 0x08

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 131
#define SAMPLE_RATE 0.01
#define RATIO (180/3.14)

volatile bool g_bMPU6050Done; //boolean that set when MPU6050 command is complete
tI2CMInstance g_sI2CMInst; // I2C master instance
int clockFreq; //Frequency of clock

//function prototypes
static void
MPU6050Callback(void *pvCallbackData, uint_fast8_t ui8Status);
void I2CIntHandler(void);
void MPU6050Example(void);
void MPU6050Example(void);
void InitI2C0(void);
void ConfigUART(void);
void Complementary_Filter(float fAccel[], float fGyro[]);

int main()
{

    //Set the clocking to run directly from the external crystal/oscillator
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
    //Configure UART to print to terminal
    ConfigUART();
    //initialize I2C module 0
    InitI2C0();
    //Get MPU6050 data
    MPU6050Example();

    return(0);
}

static void
MPU6050Callback(void *pvCallbackData, uint_fast8_t ui8Status)
//callback when transactions are completed
{
    // See if an error occurred.
    if (ui8Status != I2CM_STATUS_SUCCESS)
    {
        // An error occurred, so handle it here if required.
    }
    // Indicate that the MPU6050 transaction has completed.
    g_bMPU6050Done = true;
}

void I2CIntHandler(void)
// I2C module interrupt handler
{
    I2CMIntHandler(&g_sI2CMInst);
}

void MPU6050Example(void)
{
    float fAccel[3], fGyro[3];

    tMPU6050 sMPU6050;
    float xa = 0, ya = 0, za = 0;
    float xg = 0, yg = 0, zg = 0;

    //Initialized the MPU6050. This code assumes that the I2C master instance
    //has already been initialized.

    g_bMPU6050Done = false;
    // Initialize the MPU6050. This code assumes that the I2C master instance
    // has already been initialized.
    g_bMPU6050Done = false;
    MPU6050Init(&sMPU6050, &g_sI2CMInst, 0x68, MPU6050Callback, &sMPU6050);
    while (!g_bMPU6050Done)
    {
    }

    // Configure the MPU6050 for +/- 4 g accelerometer range.
    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_ACCEL_CONFIG, ~MPU6050_ACCEL_CONFIG_AFS_SEL_M,
                           MPU6050_ACCEL_CONFIG_AFS_SEL_4G, MPU6050Callback, &sMPU6050);
    while (!g_bMPU6050Done)
    {
    }

    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_PWR_MGMT_1, 0x00, 0b00000010 & MPU6050_PWR_MGMT_1_DEVICE_RESET, MPU6050Callback, &sMPU6050);
    while (!g_bMPU6050Done)
    {
    }

    g_bMPU6050Done = false;
    MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_PWR_MGMT_2, 0x00, 0x00, MPU6050Callback, &sMPU6050);
    while (!g_bMPU6050Done)
    {
    }

    // Loop forever reading data from the MPU6050.
    while (1)
    {
        // Request another reading from the MPU6050.
        g_bMPU6050Done = false;
        MPU6050DataRead(&sMPU6050, MPU6050Callback, &sMPU6050);
        while (!g_bMPU6050Done)
        {
        }

        // Get the new accelerometer and gyroscope readings.
        MPU6050DataAccelGetFloat(&sMPU6050, &fAccel[0], &fAccel[1], &fAccel[2]);
        MPU6050DataGyroGetFloat(&sMPU6050, &fGyro[0], &fGyro[1], &fGyro[2]);

        //Do something with the new accelerometer and gyroscope readings.

        xg = fGyro[0]*10000;
        yg = fGyro[1]*10000;
        zg = fGyro[2]*10000;
        xa = (atan2(fAccel[0], sqrt (fAccel[1] * fAccel[1] + fAccel[2] * fAccel[2]))*180.0)/3.14;
        ya = (atan2(fAccel[1], sqrt (fAccel[0] * fAccel[0] + fAccel[2] * fAccel[2]))*180.0)/3.14;
        za = (atan2(fAccel[2], sqrt (fAccel[1] * fAccel[1] + fAccel[2] * fAccel[2]))*180.0)/3.14;

        UARTprintf("ACC.  Xa: %d \t | ACC.  Ya: %d \t | ACC.  Za: %d \n", (int)xa, (int)ya, (int)za);
        UARTprintf("GYRO. Xg: %d \t | GYRO. Yg: %d \t | GYRO. Zg: %d \n", (int)xg, (int)yg, (int)zg);
        SysCtlDelay( (SysCtlClockGet()/(3*1000))*1000 ) ;
    }
}

void InitI2C0(void)
{
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.
    // I2C data transfer rate set to 400kbps.
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;

    // Initialize the I2C master driver.
    I2CMInit(&g_sI2CMInst, I2C0_BASE, INT_I2C0, 0xff, 0xff, SysCtlClockGet());

}

void ConfigUART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //enable GPIOA peripherals(the UART pins are on GPIO Port A)

    //Configure pins for the reciever and transmitter
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}

