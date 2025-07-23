
#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#define leftline         0
#define middleline       1
#define rightline        2


#include "headfile.h"

extern float t_a,t_b,t_c;
extern uint8 left_down_duandian,right_down_duandian;
extern uint8 left_duandian_up,right_duandian_up;


uint8 sqrt1(uint8 n);
int My_abs(int num);
float curvature(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3);


#endif
