/*
 * PID_controller.c
 *
 *  Created on: 04-Nov-2022
 *      Author: ESB203C
 */
#include <Control/PID_controller.h>
PID_Controller PID_Position_Control;
PID_Controller PID_Current_Control;
void Initialise_PID(PID_Controller *PID, float f32_kp, float f32_ki, float f32_kd, float f32_Kai,float f32_N, float f32_actuator_limit_P, float f32_actuator_limit_N , float f32_Ts)
{
    PID->f32_kp  = f32_kp;
    PID->f32_kd  = f32_kd;
    PID->f32_ki  = f32_ki;
    PID->f32_actuator_limit_N = f32_actuator_limit_N;
    PID->f32_actuator_limit_P = f32_actuator_limit_P;
    PID->f32_antiwindup_err = 0.0f;
    PID->f32_past_ip_diff = 0.0f;
    PID->f32_past_op_diff = 0.0f;
    PID->f32_past_ip_integrator = 0.0f;
    PID->f32_past_op_integrator = 0.0f;
    PID->f32_Ts = f32_Ts;
    PID->f32_Kai = f32_Kai;
    PID->f32_N = f32_N;
}

float PID_Control(PID_Controller *PID, float f32_err)
{
    float f32_integrator_op = 0.0f;
    float f32_differentiator_op = 0.0f;
    float f32_output = 0.0f;
    float f32_diff_const1 =  1 + PID->f32_N*PID->f32_Ts/2;
    float f32_diff_const2 =  (PID->f32_N*PID->f32_Ts/2)-1 ;
    float f32_integrator_ip = PID->f32_antiwindup_err * PID->f32_Kai;

    f32_integrator_op = PID->f32_Ts *(PID->f32_ki * f32_err + PID->f32_past_ip_integrator + f32_integrator_ip)*0.5f + PID->f32_past_op_integrator;
        f32_differentiator_op = PID->f32_N*(f32_err - PID->f32_past_ip_diff) - PID->f32_past_op_diff*f32_diff_const2;
        f32_differentiator_op = f32_differentiator_op/f32_diff_const1;
        f32_output = PID->f32_kp * f32_err + f32_integrator_op + PID->f32_kd*f32_differentiator_op;

        PID->f32_past_op_integrator  = f32_integrator_op;
        PID->f32_past_op_diff = f32_differentiator_op;
        PID->f32_past_ip_diff = f32_err;
        PID->f32_past_ip_integrator = f32_integrator_ip + PID->f32_ki *f32_err;


        if(f32_output >= PID->f32_actuator_limit_P)
        {
            PID->f32_antiwindup_err = -f32_output + PID->f32_actuator_limit_P;
            f32_output = PID->f32_actuator_limit_P;
        }
        else if(f32_output <= PID->f32_actuator_limit_N)
        {
            PID->f32_antiwindup_err = -f32_output + PID->f32_actuator_limit_N;
            f32_output = PID->f32_actuator_limit_N;
        }
        else
        {
        }
        return f32_output;
    }
