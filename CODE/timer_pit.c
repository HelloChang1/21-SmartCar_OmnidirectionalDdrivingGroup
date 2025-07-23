
#include "timer_pit.h"
int16 my_abs(int16 t) {
  if (t > 0) {
    return t;
  } else {
    return (-t);
  }
}

uint8 Start_Car_Flag = 0; //
int16 Speed_Acl_Flag = 0; //编码器距离积分标志位
uint8 diancirunflag = 0;  //用于出库时的电磁跑标志位

extern uint8 ad_left;
extern uint8 ad_right;
extern uint8 ad_middle;
extern uint8 ad_behind_left;

uint8 start_acl_flag = 0;        //角度积分标志位
uint8 cameraflag = 0;            //三岔路中标志位
uint8 sanchaflag = 0;            //三叉路进标志位
uint8 first_sancha_chu_flag = 0; //三叉路出标志位
uint8_t Circle_Pre_flag = 0;     //进环岛标志位
uint8 Circle_elect_thres = 170;  //电磁阈值

int16 Speed_Acl = 0; //编码器积分距离

// int16 firstin_delay_time=1000;
// int16 firstout_delay_time=4000;

uint8 flag_display = 0;
int16 gz = 0;

int16 GYROz_ctrl(void) {

  // int16 z=0;
  int16 GYRO_Z_last = 0;
  int16 GYRO_Z = 0;
  int16 GYRO_err = 0;

  GYRO_Z_last = GYRO_Z;
  GYRO_Z = 1.0 * (icm_gyro_z - 7) / 16.4; //计算Z轴角速度
  GYRO_err = GYRO_Z - GYRO_Z_last;
  GYRO_Z = 0.4 * GYRO_Z_last + 0.6 * GYRO_Z;
  return GYRO_Z;
}



void timer1_pit_entry(void *parameter) {

  gz = GYROz_ctrl();

  if (Speed_Acl_Flag == 0 && ad_middle < 30 && ad_behind_left < 30) {
#if 1
    SpeedCtrl(100, -100, 100, -100); //出库的速度，左出库
#else
    SpeedCtrl(-100, 100, -100, 100); //出库的速度 ，右出库
#endif
  } else {
    Speed_Acl_Flag = 1;
  }
  if (Speed_Acl_Flag == 1) {

    ForkRoad_deal(ad_middle, ad_left, ad_right); // 三叉检测
    Huan_Judge(ad_middle, ad_left, ad_right);    // 环岛检测
                                                 /*   软件延时
                                                       Firstin_Delay_Time();        //开启延时避免岔路入口误判为出口
                                                       Firstout_Delay_Time();     //环岛延时，防止第二次进环岛
                                                       banma_delay_time();        //斑马线检测延时
                                                 */
    AllCtrl();                                   //总控制

    Cancha_deal();        //第一次岔路处理
    Huan_Deal(ad_middle); //右环岛处理
    Second_Cancha_deal(); //第二次岔路处理

    Search_speed_Control(); //摄像头、电磁、自转控制循迹
  }
}

extern uint8 firstin_delay_time_flag; //开启延时标志位
extern uint8 firstin_stop_time_flag;  //停止延时
extern float Camera_Now_Angle;

extern uint8 firstout_delay_time_flag;
extern uint8 firstout_stop_time_flag;

extern uint8 bs_delay_time_flag;
extern uint8 bs_stop_time_flag;

void timer2_pit_entry(void *parameter)
//通过RT-Thread操作系统设计定时器二
//实现2s的延时效果,用于第一次进三岔路口
{
  if (firstin_delay_time_flag == 1) {
    {
      Camera_Now_Angle = 0;
      firstin_stop_time_flag = 1;
    }
  }
}

void timer3_pit_entry(void *parameter)
//通过RT-Thread操作系统设计定时器三
//实现4s的延时效果,用于第一次出三岔路口
{

  if (firstout_delay_time_flag == 1) {

    { firstout_stop_time_flag = 1; }
  }
}

void timer4_pit_entry(void *parameter)
//通过RT-Thread操作系统设计定时器四
//实现4s的延时效果,用于斑马线的延时
{
  if (bs_delay_time_flag == 1) {
    { bs_stop_time_flag = 1; }
  }
}

void timer5_pit_entry(void *parameter)
//通过RT-Thread操作系统设计定时器五，用于在元素检测中计算量过大，计算时间长
//定时器的周期过小，不能完成赛道元素的检测
{
  if (Speed_Acl_Flag == 1) {
    ForkRoad_deal(ad_middle, ad_left, ad_right); // 三叉检测
    Huan_Judge(ad_middle, ad_left, ad_right);    // 环岛检测
  }
}

void timer_pit_init(void) {
  rt_timer_t timer;
  rt_timer_t t2;
  rt_timer_t t3;
  rt_timer_t t4;
  rt_timer_t t5;

  //创建四个个定时器 周期运行
  timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 2,
                          RT_TIMER_FLAG_PERIODIC);
  t2 = rt_timer_create("timer2", timer2_pit_entry, RT_NULL, 2000,
                       RT_TIMER_FLAG_PERIODIC);
  t3 = rt_timer_create("timer3", timer3_pit_entry, RT_NULL, 4000,
                       RT_TIMER_FLAG_PERIODIC);
  t4 = rt_timer_create("timer4", timer4_pit_entry, RT_NULL, 4000,
                       RT_TIMER_FLAG_PERIODIC);
  t5 = rt_timer_create("timer5", timer5_pit_entry, RT_NULL, 8,
                       RT_TIMER_FLAG_PERIODIC);
  //启动定时器
  if (RT_NULL != timer) {
    rt_timer_start(timer);
  }
  if (RT_NULL != t2) {
    rt_timer_start(t2);
  }
  if (RT_NULL != t3) {
    rt_timer_start(t3);
  }
  if (RT_NULL != t4) {
    rt_timer_start(t4);
  }
  if (RT_NULL != t5) {
    rt_timer_start(t5);
  }
}
