#include <Control/Current_Control_Loop.h>
#include <Control/Position_Control_Loop.h>
#include "stdint.h"
#include "PID_controller.h"
#include "Filter.h"
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "uartstdio.h"

void Current_Control_Loop(float Current_Setpoint,float Current_ADC_data)
{
    uint32_t period;
    static uint32_t Current_frame_count = 0;
    float Current_set_point = Current_Setpoint;
    float Current_measured_val = Current_ADC_data;
    float Current_PID_out =0.0f;;

    Current_frame_count++;
    Current_set_point = (Current_set_point * Current_frame_count)/5000;

    if(Current_frame_count >= 5000)
    {
        Current_frame_count = 5000;
    }
    float Current_err = Current_set_point - Current_measured_val;

    Current_PID_out = PID_Control(&PID_Current_Control, Current_err);

    if(Current_PID_out >0)
    {
        Current_PID_out = 0;
    }
    else if(Current_PID_out < -25)
    {
        Current_PID_out = -25;
    }
    Current_PID_out = Current_PID_out/(25);

    period = (uint32_t)(12000+Current_PID_out*12000);
    MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,period);
}

