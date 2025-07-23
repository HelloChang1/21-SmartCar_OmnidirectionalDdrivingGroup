#ifndef _ELEMENTS_H_
#define _ELEMENTS_H_

#include "headfile.h"

extern int16 total;

extern uint8 out_cross_flag;
extern uint8 Fork_Pre_flag,Fork_flag;
extern uint8 flag_starting_line;
extern uint8 flag_enter_starting_line;
extern uint8 flag_right_starting_line;
extern uint8 flag_left_starting_line;
extern uint8 flow;
extern uint8 zhidao_flag;
extern uint8 l_bend_flag;
extern uint8 r_bend_flag;

void WD_judge(void );
void chalupanduan();
void WD_judge();
void check_starting_line();
#endif
