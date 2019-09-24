#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

uint8_t ui8PinData=2; //R = 2, G = 8, B = 4

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_15|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //delay = Delay time(seconds)/(1/SysClk*3) where SysClk = (400M/2)/15 and Delay time = 0.5 s

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Selecting GPIO pins as output pins

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, ui8PinData);
        SysCtlDelay(2222222.222); //calculated delay on time
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
        SysCtlDelay(2222222.222); //calculated delay off time
        if(ui8PinData==2) {ui8PinData=8;} //R => current state
        else if (ui8PinData==8) {ui8PinData=4;} //G
        else if (ui8PinData==4) {ui8PinData=10;} //B
        else if (ui8PinData==10) {ui8PinData=12;} //RB
        else if (ui8PinData==12) {ui8PinData=14;} //GB
        else if (ui8PinData==14) {ui8PinData=2;} //RGB
    }
}
