/*
 * Filter.c
 *
 *  Created on: 04-Nov-2022
 *      Author: ESB203C
 */
#include <Control/Filter.h>
IIR_Second_order_Filter Filter_Notch_1;
IIR_Second_order_Filter Filter_BP_1;
IIR_Second_order_Filter Filter_Notch_2;
IIR_Second_order_Filter Filter_BP_2;
void ASW_IIR_second_order_init(IIR_Second_order_Filter *IIR_Second_order, float f32_num_coeff_0,float f32_num_coeff_1,float f32_num_coeff_2, float f32_den_coeff_0,float f32_den_coeff_1)
{
        IIR_Second_order->f32_den_coeff[0] = f32_den_coeff_0;
        IIR_Second_order->f32_den_coeff[1] = f32_den_coeff_1;
        IIR_Second_order->f32_num_coeff[0] = f32_num_coeff_0;
        IIR_Second_order->f32_num_coeff[1] = f32_num_coeff_1;
        IIR_Second_order->f32_num_coeff[2] = f32_num_coeff_2;

        IIR_Second_order->f32_past_input[0] = 0.0f;
        IIR_Second_order->f32_past_input[1] = 0.0f;
        IIR_Second_order->f32_past_output[0] = 0.0f;
        IIR_Second_order->f32_past_output[1] = 0.0f;

}

float ASW_IIR_second_order(IIR_Second_order_Filter *IIR_Second_order, float f32_input)
{
    float f32_output = 0.0f;
    f32_output = (IIR_Second_order->f32_num_coeff[0] * f32_input ) + (IIR_Second_order->f32_num_coeff[1] * IIR_Second_order->f32_past_input[1]) + (IIR_Second_order->f32_num_coeff[2] * IIR_Second_order->f32_past_input[0])
    + (-IIR_Second_order->f32_den_coeff[0] * IIR_Second_order->f32_past_output[1]) + (-IIR_Second_order->f32_den_coeff[1] * IIR_Second_order->f32_past_output[0]);

    IIR_Second_order->f32_past_input[0] = IIR_Second_order->f32_past_input[1];
    IIR_Second_order->f32_past_input[1] = f32_input;
    IIR_Second_order->f32_past_output[0] = IIR_Second_order->f32_past_output[1];
    IIR_Second_order->f32_past_output[1] = f32_output;

    return f32_output;
}



