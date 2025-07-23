#ifndef  _TEST_H
#define _TEST_H


#include "headfile.h"

#define  Angle_T 0.002

#define S_MOTOR_PIN   PWM2_CH1_A0       //定义舵机引脚


extern int16 gkp;
extern int16 speedy;
extern int16 speedz;
extern int16 speedx;
extern int16 Mode;





extern  uint8 dianci_judge_flag ;

 void ForkRoad_deal(uint16 mid,uint16 left_1,uint16 right_1);
 void Huan_Judge(uint16 mid,uint16 left_1,uint16 right_1);
 //void Firstin_Delay_Time(void );
 //void Firstout_Delay_Time(void );
// void banma_delay_time(void );
 void Cancha_deal(void );
 void Huan_Deal(uint16 mid);
 void Search_speed_Control(void);
 void AllCtrl();
 void speedz_ctrl(void);

 void Second_Cancha_deal(void);


 void Total_speed_ctrl(void);
 void Second_Judge(void);
#endif
