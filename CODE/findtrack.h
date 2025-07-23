#ifndef __FINDTRACK_H__
#define __FINDTRACK_H__



#include "headfile.h"

/*** 宏定义 ****/
#define Black_Point 0
#define White_Point 255
#define RowMax	    60	  //行数
#define ColumnMax	120	  //列数
#define cancel_hang    12

/*** 外部函数变量申明 ****/  

extern uint8 mid;
extern uint8 r_start_r,l_start_l;

//定义存储接收图像的数组,CAMERA_H(59-0),CAMERA_W(0-79)

extern int   AllLose;
extern float AvaliableLines;


extern int   LeftLoseStart  ;  // 记录左边丢线的开始行
extern int   RightLoseStart ;  // 记录右边边丢线的开始行

extern  int   RightEdge[RowMax+1]; //左边线点数组
extern  int   LeftEdge[RowMax+1];  //右边线点数组
extern  int   MiddleLine[RowMax+1];//中间线点数组 

extern int   LastLine;             //上一行 
extern  int LastLastLine;          //上上一行 
extern int   Width[RowMax+1];      //静态路宽 
extern int   LeftLose;             //左边线丢失数目 
extern int   RightLose ;           //右边线丢失数目 

/*** 函数声明 ****/    
extern void SetInitVal();          //中线搜索函数初始化 
extern void judge_eg();
extern uint8 l_start,r_start,break_row;
int MAX(int m, int n) ;

#endif
