
#include <Control/Current_Control_Loop.h>
#include <Control/Position_Control_Loop.h>
#include "stdint.h"
#include "PID_controller.h"
#include "Filter.h"
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "uartstdio.h"

void Position_Control_Loop(float Position_ADC_data,float Current_ADC_data)
{
    static uint32_t Position_frame_count = 0;
    float Position_set_point = 0.008f;
    float Position_measured_val =0.015f- Position_ADC_data * POSITION_ENG_CONVERSION;
    float Current_Setpoint =0.0f;
    Position_frame_count++;
    Position_set_point = 0.012 + ((Position_set_point-0.012) * Position_frame_count/5000);
    if(Position_frame_count >= 5000)
    {
        Position_frame_count = 5000;
    }
    float Position_err = Position_set_point - Position_measured_val;

    Current_Setpoint = PID_Control(&PID_Position_Control, Position_err);

    if(Current_Setpoint >0)
    {
        Current_Setpoint = 0;
    }
    else if(Current_Setpoint < -25)
    {
        Current_Setpoint = -25;
    }
    Current_Control_Loop(Current_Setpoint,Current_ADC_data);
}

