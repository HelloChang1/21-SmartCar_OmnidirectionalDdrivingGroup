#ifndef _motor_h
#define _motor_h

#include "headfile.h"

#define MOTOR1_A   B10              //定义1电机正反转引脚
#define MOTOR1_B   PWM4_CH3_B8      //定义1电机PWM引脚

#define MOTOR2_A   B11              //定义2电机正反转引脚
#define MOTOR2_B   PWM4_CH4_B9      //定义2电机PWM引脚

#define MOTOR3_A   A15              //定义3电机正反转引脚
#define MOTOR3_B   PWM4_CH1_B6      //定义3电机PWM引脚

#define MOTOR4_A   B3               //定义4电机正反转引脚
#define MOTOR4_B   PWM4_CH2_B7      //定义4电机PWM引脚


#define ATOM_PWM_MAX 6000
#define ATOM_error_MAX 300



#define PI   3.14

typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                imax;       //积分限幅
    float                out_p;  //KP输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out;    //pid输出
    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次误差与上上次误差之差
    uint32_t             last_t;     //< 上次时间
}pid_param_t;




void motor_init(void);
void motor_pid(int16 expect_speed);
void MotorCtrl(int16 motor1, int16 motor2, int16 motor3, int16 motor4);
    
#endif
