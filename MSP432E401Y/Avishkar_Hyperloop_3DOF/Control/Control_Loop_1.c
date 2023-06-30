/*
 * Control_Loop.c
 *
 *  Created on: 04-Nov-2022
 *      Author: ESB203C
 */
#include <Control/Control_Loop_1.h>
#include <Control/Filter.h>
#include <Control/PID_controller.h>
#include "stdint.h"
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "uartstdio.h"
void Control_Loop_1(float f32_ADC_data)
{
    static uint32_t ui32_frame_count = 0;
    float f32_set_point = 8.0f/1000.0f;
    float f32_measured_val =0.015f- f32_ADC_data * POSITION_ENG_CONVERSION_1;
    float PID_out =0.0f;
    float Notch_out =0.0f;

    uint32_t ui32_period;
    ui32_frame_count++;

    f32_set_point = 0.01f;

    f32_set_point = 0.012 + ((f32_set_point-0.012) * ui32_frame_count/100000);


    if(ui32_frame_count >= 100000)
    {
        ui32_frame_count = 100000;
    }
    float f32_err = f32_set_point - f32_measured_val;

    PID_out = PID_Control(&PID_Position_Control_1, f32_err);
    Notch_out=PID_out;

    /*Code to convert to duty cycle*/
    if(Notch_out >0)
    {
        Notch_out = 0;
    }
    else if(Notch_out < -24.99)
    {
        Notch_out = -24.99;
    }
    Notch_out = Notch_out/(25);

    ui32_period = (uint32_t)(12000+Notch_out*12000);

    /*Update the PWM*/
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,ui32_period);
}

