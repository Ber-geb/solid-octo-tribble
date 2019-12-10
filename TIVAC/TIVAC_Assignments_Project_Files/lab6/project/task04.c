#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/qei.h"
#include "driverlib/adc.h"
#include "driverlib/qei.h"


#define PWM_FREQUENCY 50

volatile int qeiPosition;

int main(void)
{
    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    volatile int qeiPosition;
    volatile uint32_t ui8Adjust[4];
    volatile uint32_t ui8Avg;

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1); //SS1
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1);
    ROM_ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);

    ROM_ADCSequenceEnable(ADC0_BASE, 1);

    ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2); //PE2 AIN1


    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    ROM_GPIOPinConfigure(GPIO_PD0_M1PWM0);

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

    while (1) //This is the main loop of the program
    {
        ROM_ADCIntClear(ADC0_BASE, 1);
        ROM_ADCProcessorTrigger(ADC0_BASE, 1);

        while(!ROM_ADCIntStatus(ADC0_BASE, 1, false))
        {
        }

        ROM_ADCSequenceDataGet(ADC0_BASE, 1, ui8Adjust);
        ui8Avg = (ui8Adjust[0] + ui8Adjust[1] + ui8Adjust[2] + ui8Adjust[3] + 2)/4;
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Avg * ui32Load / 1000);


    }


}
