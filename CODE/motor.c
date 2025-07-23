//----------------------------------------------------------------------------------
/*
 * 自己编写的中断函数
 * 四个电机，使用一个定时器。（电机使用的那个定时器） 【通用定时器】(定时器(4)
 * 舵机使用一个定时器。                             （先不考虑）
【通用定时器】(定时器3)
 * 摄像头像素时钟信号获取使用一个定时器。 （先不考虑）           【通用定时器】
(定时器1)
 * 周期中断使用一个定时器。                       （用定时器）
【高级定时器】   (定时器2) （编码器和ICM的分时复用）
 *
 * 7 个定时器：
 *
-                  1 个 16 位高级定时器，
      包含通用定时器功能，并自带死区控制和紧急刹车，提供用于电机控制的 PWM

-                  3 个 16 位通用定时器，
      提供多达 4 个用于输入捕获/输出比较/PWM/脉冲计数的通道和增量编码器输入

-                  2 个看门狗定时器（独立和窗口型）

-                  系统时间定时器：64 位自增型计数器
 *
 *
 *电机的PWM使用17K这样可以劲量避免PWM干扰到电磁采集，
 *——因为中间的电磁引导线通的是20kHz的交变电流，避免了PWM波受到影响
 *
 *本文件的程序应该放在周期中断（PIT）内保证执行的周期是固定的
 *
 *需要根据车模的实际情况确定好，哪个PWM波通道是控制电机前进的，哪个是控制
 *控制电机后退的，最简单的方法就是将挨个设置PWM通道的值为20%的占空比，然后运行
 *查看车轮的转动情况。
 *
 */
//-----------------------------------------------------------------------------------

#include "motor.h"

/* 四个轮子的实际速度  */
int16 realSpeed1 = 0;
int16 realSpeed2 = 0;
int16 realSpeed3 = 0;
int16 realSpeed4 = 0;

pid_param_t pid1;
pid_param_t pid2;
pid_param_t pid3;
pid_param_t pid4;

float constrain_float(float amt, float low, float high) {
  return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

void PidInit(pid_param_t *pid) {
  pid->kp = 0;
  pid->ki = 0;
  pid->kd = 0;
  pid->imax = 0;
  pid->out_p = 0;
  pid->out_i = 0;
  pid->out_d = 0;
  pid->out = 0;
  pid->integrator = 0;
  pid->last_error = 0;
  pid->last_derivative = 0;
  pid->last_t = 0;
}

float PidLocCtrl(pid_param_t *pid, float error) {
  /* 累积误差 */
  pid->integrator += error;

  /* 误差限幅 */
  constrain_float(pid->integrator, -pid->imax, pid->imax);

  pid->out_p = pid->kp * error;
  pid->out_i = pid->ki * pid->integrator;
  pid->out_d = pid->kd * (error - pid->last_error);

  pid->last_error = error;

  pid->out = pid->out_p + pid->out_i + pid->out_d;

  return pid->out;
}

/*!
  * @brief    pid增量式控制器输出
  *
  * @param    pid     pid参数
  * @param    error   pid输入误差
  *
  * @return   PID输出结果   注意输出结果已经包涵了上次结果
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
float PidIncCtrl(pid_param_t *pid, float error) {

  pid->out_p = pid->kp * (error - pid->last_error);
  pid->out_i = pid->ki * error;
  pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

  pid->last_derivative = error - pid->last_error;
  pid->last_error = error;

  pid->out += pid->out_p + pid->out_i + pid->out_d;

  return pid->out;
}

/*
 * 电机pid参数        对应电机如下
 *
 *     3(左前)   2(右前)
 *
 *     4(左后)   1(右后)
 */

void motor_init(void) {
  //初始化电机PWM引脚和方向引脚

  //桌大大的推文中，建议电磁组电机频率选用13K-17K
  //最大占空比值PWM_DUTY_MAX 可以在fsl_pwm.h文件中修改 默认为50000
  //对于一个PWM模块 包含的所有通道只能输出频率一样 占空比不一样的 PWM
  //CH32V103R8T6只有四个PWM模块 每个模块有4个通道
  gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
  pwm_init(MOTOR1_B, 17000, 0);
  gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
  pwm_init(MOTOR2_B, 17000, 0);
  gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
  pwm_init(MOTOR3_B, 17000, 0);
  gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
  pwm_init(MOTOR4_B, 17000, 0);

  PidInit(&pid1);
  PidInit(&pid2);
  PidInit(&pid3);
  PidInit(&pid4);

  /* PID 参数需要自行调节 */
  // 0.1-1.4
  // 0.075-4
  // 3-0.03
  pid1.kp = 5;
  pid1.ki = 0.008;
  pid1.kd = 0;

  pid2.kp = 5;
  pid2.ki = 0.008;
  pid2.kd = 0;

  pid3.kp = 6;
  pid3.ki = 0.008;
  pid3.kd = 0;

  pid4.kp = 6;
  pid4.ki = 0.008;
  pid4.kd = 0;
}

void MotorCtrl(int16 motor1, int16 motor2, int16 motor3, int16 motor4) {

  /* 限幅  */
  constrain_float(motor1, -ATOM_PWM_MAX, ATOM_PWM_MAX);
  constrain_float(motor2, -ATOM_PWM_MAX, ATOM_PWM_MAX);
  constrain_float(motor3, -ATOM_PWM_MAX, ATOM_PWM_MAX);
  constrain_float(motor4, -ATOM_PWM_MAX, ATOM_PWM_MAX);

  if (0 <=
      motor1) //电机1   正转 设置占空比为 百分之 (1000/TIMER1_PWM_DUTY_MAX*100)
  {
    gpio_set(MOTOR2_A, 1);
    pwm_duty(MOTOR2_B, motor1);
  } else //电机1   反转
  {
    gpio_set(MOTOR2_A, 0);
    pwm_duty(MOTOR2_B, -motor1);
  }

  if (0 <= motor2) //电机2   正转
  {
    gpio_set(MOTOR1_A, 1);
    pwm_duty(MOTOR1_B, motor2);
  } else //电机2   反转
  {
    gpio_set(MOTOR1_A, 0);
    pwm_duty(MOTOR1_B, -motor2);
  }

  if (0 <= motor3) //电机3   正转
  {
    gpio_set(MOTOR4_A, 1);
    pwm_duty(MOTOR4_B, motor3);
  } else //电机3   反转
  {
    gpio_set(MOTOR4_A, 0);
    pwm_duty(MOTOR4_B, -motor3);
  }

  if (0 <= motor4) //电机3   正转
  {
    gpio_set(MOTOR3_A, 1);
    pwm_duty(MOTOR3_B, motor4);
  } else //电机3   反转
  {
    gpio_set(MOTOR3_A, 0);
    pwm_duty(MOTOR3_B, -motor4);
  }
}

#define K_lefton 8.636
#define K_righton 8.636
#define K_rightdown 8.636
#define K_leftdown 8.636

void SpeedCtrl(int16 targetSpeed1, int16 targetSpeed2, int16 targetSpeed3,
               int16 targetSpeed4) {

  int16 pwm1 = 0;
  int16 pwm2 = 0;
  int16 pwm3 = 0;
  int16 pwm4 = 0;

  /* PID处理 */
  PidIncCtrl(&pid1, targetSpeed1 - realSpeed1);
  PidIncCtrl(&pid2, targetSpeed2 - realSpeed2);
  PidIncCtrl(&pid3, targetSpeed3 - realSpeed3);
  PidIncCtrl(&pid4, targetSpeed4 - realSpeed4);

  pwm1 = K_lefton * pid1.out;
  pwm2 = K_righton * pid2.out;
  pwm3 = K_rightdown * pid3.out;
  pwm4 = K_leftdown * pid4.out;

  MotorCtrl(pwm1, pwm2, pwm3, pwm4);
}

/*----------------------------------------------------摄像头部分数据处理--------------------------------------*/
int16 Caculate_piancha = 0;
float Error_Kp = 0.15;
float Error_Kd = -0.00003;
int16 Error_Calculate() {
  int16 piancha = 0;
  int16 last_piancha = 0;
  //第一次，5-40
  //第二次，20-55
  for (int i = 15; i <= 45; i++)

  {
    piancha += MiddleLine[i] - 60;
  }
  last_piancha = piancha;
  Caculate_piancha = Error_Kp * piancha + Error_Kd * (piancha - last_piancha);

  return Caculate_piancha;
}

/*----------------------------------------------------电磁部分数据处理--------------------------------------*/
float dianci_kp = -5.5;
extern uint8 ad_left;
extern uint8 ad_right;
int16 ad_sum;
int16 ad_diff;

int16 ADC_Error_Calculate_Second() {
  int16 t = 0;
  int16 position1 = 0;
  ad_sum = (ad_left) + (ad_right);             //计算电感之和
  ad_diff = (int16)((ad_left)) - ((ad_right)); //计算电感之差
  position1 = (ad_diff << 7) / (ad_sum + 1); //计算差比和数值 并放大128倍
  t = dianci_kp * position1;
  return t;
}

/*----------------------------------------------------运动总底层控制处理--------------------------------------*/
int16 targetspeed_leftup = 0;
int16 targetspeed_leftdown = 0;
int16 targetspeed_rightup = 0;
int16 targetspeed_rightdown = 0;

extern uint8 highspeed_k_flag;
extern uint8 lowspeed_k_flag;
extern uint8 stopspeed_k_flag;

void Speed_Controler(int16 Vx, int16 Vy, int16 Vw, int16 mode) {
  int16 SpeedSet1 = 0;
  int16 SpeedSet2 = 0;
  int16 SpeedSet3 = 0;
  int16 SpeedSet4 = 0;

  if (mode == 0) //三岔刹车
  {
    targetspeed_leftup = 0;
    targetspeed_rightup = 0;
    targetspeed_rightdown = 0;
    targetspeed_leftdown = 0;
  }

  if (mode == 1) // 电磁循迹
  {
    SpeedSet1 = Vx + Vy - Vw;
    SpeedSet2 = -Vx + Vy + Vw;
    SpeedSet3 = Vx + Vy + Vw;
    SpeedSet4 = -Vx + Vy - Vw;

    uint8 k = 0;
    float k1 = 0;
    if (highspeed_k_flag == 1) {
      k = 1.5;
      k1 = 1;
    }
    if (lowspeed_k_flag == 1) {
      k = 1;
      k1 = 1;
    }
    if (stopspeed_k_flag == 1) {
      k = -0.01;
      k1 = -0.01;
    }

    targetspeed_leftup =
        (SpeedSet1 + k1 * ADC_Error_Calculate_Second()); //左轮差速
    targetspeed_rightup =
        (SpeedSet2 - k1 * ADC_Error_Calculate_Second()); //右轮差速
    targetspeed_rightdown =
        (SpeedSet3 - k * ADC_Error_Calculate_Second()); //左轮差速
    targetspeed_leftdown =
        (SpeedSet4 + k * ADC_Error_Calculate_Second()); //右轮差速

    if (targetspeed_leftup < 0) {
      targetspeed_leftup = 20;
    }
    if (targetspeed_rightup < 0) {
      targetspeed_rightup = 20;
    }
    if (targetspeed_rightdown < 0) {
      targetspeed_rightdown = 20;
    }
    if (targetspeed_leftdown < 0) {
      targetspeed_leftdown = 20;
    }
  }
  if (mode == 2) //摄像头横向循迹
  {
    SpeedSet1 = Vx + Vy - Vw;
    SpeedSet2 = -Vx + Vy + Vw;
    SpeedSet3 = Vx + Vy + Vw;
    SpeedSet4 = -Vx + Vy - Vw;

    targetspeed_leftup = (SpeedSet1 + Error_Calculate());    //左轮差速
    targetspeed_rightup = (SpeedSet2 - Error_Calculate());   //右轮差速
    targetspeed_rightdown = (SpeedSet3 - Error_Calculate()); //左轮差速
    targetspeed_leftdown = (SpeedSet4 + Error_Calculate());  //右轮差速
  }
  if (mode == 3) {
    SpeedSet1 = Vx + Vy - Vw;
    SpeedSet2 = -Vx + Vy + Vw;
    SpeedSet3 = Vx + Vy + Vw;
    SpeedSet4 = -Vx + Vy - Vw;

    targetspeed_leftup = (SpeedSet1 + Error_Calculate());    //左轮差速
    targetspeed_rightup = (SpeedSet2 - Error_Calculate());   //右轮差速
    targetspeed_rightdown = (SpeedSet3 - Error_Calculate()); //左轮差速
    targetspeed_leftdown = (SpeedSet4 + Error_Calculate());  //右轮差速
  }

  SpeedCtrl(targetspeed_leftup, targetspeed_rightup, targetspeed_rightdown,
            targetspeed_leftdown);
}
