/*
 *                MSP432E401Y
 *             ------------------
 *         /|\|               PE3|<-- AIN0
 *          | |               PE2|<-- AIN1
 *          --|RST            PE1|<-- AIN2
 *            |               PE0|<-- AIN3
 *            |                  |
 *            |               PG0|--> PWM OUT (1.8 kHZ 50% DC)
 *            |                  |
 *            |               PA0|<-- U0RX
 *            |               PA1|--> U0TX
 */


#include <Control/Current_Control_Loop.h>
#include <Control/Position_Control_Loop.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "uartstdio.h"
#include "Control/Application_Init.h"

uint32_t getADCValue[4];
volatile bool bgetConvStatus = false;


void ADC0SS2_IRQHandler(void)
{
    static uint32_t frame_counter = 0;
    float Position_ADC_data = 0;
    float Current_ADC_data=0;
    uint32_t getIntStatus;
    frame_counter++;
    /* Get the interrupt status from the ADC */
    getIntStatus = MAP_ADCIntStatus(ADC0_BASE, 2, true);

    /* If the interrupt status for Sequencer-2 is set the
     * clear the status and read the data */
    if (getIntStatus == 0x4)
    {
        /* Clear the ADC interrupt flag. */
        MAP_ADCIntClear(ADC0_BASE, 2);

        /* Read ADC Value. */
        MAP_ADCSequenceDataGet(ADC0_BASE, 2, getADCValue);

        bgetConvStatus = true;
    }
    Position_ADC_data = getADCValue[0] * 3.3 / (4096);
    Current_ADC_data = getADCValue[1];
    if (frame_counter >= 5000)
    {
        if (frame_counter == 5000)
        {
            MAP_GPIOPinConfigure(GPIO_PG0_M0PWM4);
            MAP_GPIOPinConfigure(GPIO_PG1_M0PWM5);
            MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        }
        Position_Control_Loop(Position_ADC_data,Current_ADC_data);
        frame_counter = 50000;
    }

}

void ConfigureUART(uint32_t systemClock)
{
    /* Enable the clock to GPIO port A and UART 0 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Configure the GPIO Port A for UART 0 */
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure the UART for 115200 bps 8-N-1 format */
    UARTStdioConfig(0, 115200, systemClock);
}

int main(void)
{
    uint32_t systemClock;

    /* Configure the system clock for 120 MHz */
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                         120000000);

    /* Initialize serial console */
    ConfigureUART(systemClock);

    /* Enable the clock to GPIO Port E and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)))
    {
    }

    /* Configure PE0-PE3 as ADC input channel */
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

    /* Enable the clock to ADC-0 and wait for it to be ready */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while (!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)))
    {
    }

    /* Configure Sequencer 2 to sample the analog channel : AIN0-AIN3. The
     * end of conversion and interrupt generation is set for AIN3 */
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH1);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH2);
    MAP_ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH3 | ADC_CTL_IE |
    ADC_CTL_END);

    /* Enable sample sequence 2 with a PWM signal trigger.  Sequencer 2
     * will do a single sample when the PWM generates a match for the Duty
     * Cycle */
    MAP_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PWM2, 2);

    /* Since sample sequence 2 is now configured, it must be enabled. */
    MAP_ADCSequenceEnable(ADC0_BASE, 2);

    /* Clear the interrupt status flag before enabling. This is done to make
     * sure the interrupt flag is cleared before we sample. */
    MAP_ADCIntClear(ADC0_BASE, 2);
    MAP_ADCIntEnable(ADC0_BASE, 2);

    /* Enable the Interrupt generation from the ADC-0 Sequencer */
    MAP_IntEnable(INT_ADC0SS2);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    while (!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG)))
    {
    }

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while (!(MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0)))
    {
    }

    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 24000);
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 12000);
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 12000);
    MAP_PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_2, PWM_TR_CNT_ZERO);
    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, true);
    MAP_PWMDeadBandEnable(PWM0_BASE, PWM_GEN_2, 50, 50);
    MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);

    Application_Init();

    while (1)
    {
        while (!bgetConvStatus);
        bgetConvStatus = false;

    }
}
