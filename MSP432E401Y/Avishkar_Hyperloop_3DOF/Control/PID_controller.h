/*
 * PID_controller.h
 *
 *  Created on: 04-Nov-2022
 *      Author: ESB203C
 */

#ifndef CONTROL_PID_CONTROLLER_H_
#define CONTROL_PID_CONTROLLER_H_

#define PID_CONTROLLER_DATA_LEN 2

typedef struct
{
    float f32_kp;
    float f32_ki;
    float f32_kd;
    float f32_past_ip_integrator;
    float f32_past_op_integrator;
    float f32_past_ip_diff;
    float f32_past_op_diff;
    float f32_actuator_limit_P;
    float f32_actuator_limit_N;
    float f32_antiwindup_err;
    float f32_Ts;
    float f32_Kai;
    float f32_N;
}PID_Controller;

extern PID_Controller PID_Position_Control_1;
extern PID_Controller PID_Position_Control_2;

void Initialise_PID(PID_Controller *PID, float f32_kp, float f32_ki, float f32_kd,  float f32_Kai, float f32_N, float f32_actuator_limit_P, float f32_actuator_limit_N  , float f32_Ts);
float PID_Control(PID_Controller *PID, float f32_err);
#endif /* CONTROL_PID_CONTROLLER_1_H_ */
