/*
 * Filter.h
 *
 *  Created on: 04-Nov-2022
 *      Author: ESB203C
 */

#ifndef CONTROL_FILTER_H_
#define CONTROL_FILTER_H_

typedef struct filter3
{
    float f32_past_output[2u];
    float f32_past_input[2u];
    float f32_num_coeff[3u];
    float f32_den_coeff[2u];

} IIR_Second_order_Filter;

extern IIR_Second_order_Filter Filter_Notch_1;
extern IIR_Second_order_Filter Filter_BP_1;
extern IIR_Second_order_Filter Filter_Notch_2;
extern IIR_Second_order_Filter Filter_BP_2;
void ASW_IIR_second_order_init(IIR_Second_order_Filter *IIR_Second_order, float f32_num_coeff_0, float f32_num_coeff_1, float f32_num_coeff_2, float f32_den_coeff_0, float f32_den_coeff_1);
float ASW_IIR_second_order(IIR_Second_order_Filter *IIR_Second_order, float f32_input);
#endif /* CONTROL_FILTER_1_H_ */
