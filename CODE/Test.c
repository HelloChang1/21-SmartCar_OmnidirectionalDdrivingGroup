#include"Test.h"


extern int16 Speed_Acl;

extern uint8 ad_left;
extern uint8 ad_right;
extern uint8 ad_middle;
extern uint8 ad_behind_left;
extern float Now_Angle;

uint8 Dianci_Or_Camera_flag=0;
uint8 startline_flag=0;
/*----------------------------------------------*/
uint8 firstin_delay_time_flag=0;  //开启延时标志位
uint8  firstin_stop_time_flag=0;  //停止延时
extern int16  firstin_delay_time ; // 延时时间
/*-------------------------------*/

extern uint8 Second_dianci_judge_flag;


uint8 secin_delay_time_flag=0;
int   secin_delay_time=2000;
 uint8    secin_stop_time_flag=0;

/*----------------------------------------------*/
uint8 firstout_delay_time_flag;   //第一次出岔路延时
uint8 firstout_stop_time_flag;
extern int16 firstout_delay_time;
/*-------------------------------*/

/*----------------------------------------------*/
uint8 HuanDao_flag=0;    //环岛标志位
uint8 Right_Or_Left_Huan_flag=0;  //左环为1，右环为2
/*-------------------------------*/

extern  int16 gz;//获取Z轴低通滤波以后的Z轴角速度
 uint8 highspeed_k_flag;
 uint8 lowspeed_k_flag ;
 uint8 stopspeed_k_flag;
 float Now_Angle =0;         //陀螺仪积分角度
float Camera_Now_Angle=0;
float Second_In_Now_Angle=0;
float Three_Now_Angle=0;


 int16 gkp=0.8;
 int16 speedy=0;
 int16 speedz=0;
 int16 speedx=0;
 int16 Mode=0;


uint8  bs_delay_time_flag=0;
uint8  bs_stop_time_flag=0;
int16 bs_delay_time=2000;
void banma_delay_time(void )
{
    if(bs_delay_time_flag==1)
        {
            bs_delay_time--;
         if(bs_delay_time<=0)
            {
                bs_stop_time_flag=1;
            }
        }
    }



//电感区分十字与三岔
uint8 dianci_judge_flag=0;
uint8 Second_dianci_judge_flag=0;
void ForkRoad_deal(uint16 mid,uint16 left_1,uint16 right_1)
{
if(dianci_judge_flag==0&&HuanDao_flag==0)
{


     static  int16 last_value[9];    //创建一个数组队列
     static  int16 left_value[5];    //创建一个数组队列
     static  int16 right_value[5];   //创建一个数组队列
     uint8 i=0;
     uint8 x;

     for(i=0;i<8;i++)
     {
        last_value[i]=last_value[i+1];
     }
     last_value[8]=mid;

     for(i=0;i<4;i++)
     {
          left_value[i]=left_value[i+1];

      }
     left_value[4]=left_1;

     for(i=0;i<4;i++)
     {
          right_value[i]=right_value[i+1];

     }
    right_value[4]=right_1;

      if( (last_value[8]<last_value[4])&& (last_value[4]<last_value[0]))// &&(ForkRoad_Flag==0)&&(ForkRoad_Angle_Flag==0) && (flag_huandao_pre==0) &&(Flag_360==0))
      {
          if( (left_value[4]<left_value[2]) &&(left_value[2]<left_value[0])&&(right_value[4]<right_value[2])&&(right_value[2]<right_value[0])  )
          {
              x=(last_value[8]+last_value[7]+last_value[6]+last_value[5]+last_value[4]+last_value[3]+last_value[2]+last_value[1]+last_value[0])/9;
                       if(x<=80)

                         {
                           Now_Angle=0;
                             dianci_judge_flag=1;
                             rt_mb_send(buzzer_mailbox, 100);  //根据蜂鸣器创建的线程以及邮箱
                             //代替传统的裸机编程
                             //通过向蜂鸣器邮箱发送邮件的形式，用于检测到三岔路时，使蜂鸣器发生，用于判断是否成功识别
                         }

          }

     }
}
}


 void speedz_ctrl(void)
 {

     int16 z=0;
     int16 GYRO_Z_last=0;
     int16 GYRO_Z=0;
     int16 GYRO_err=0;

     GYRO_Z_last = GYRO_Z;
     GYRO_Z = 1.0 * (icm_gyro_z - 8)/ 16.4; //计算Z轴角速度
     GYRO_err = GYRO_Z - GYRO_Z_last;
     GYRO_Z = 0.4 * GYRO_Z_last + 0.6 * GYRO_Z;
     speedz=gkp*GYRO_Z;

    speedx=-1*ADC_Error_Calculate_Second();
     }


 void AllCtrl()
 {
     if(HuanDao_flag==1&&dianci_judge_flag==1)
     {
         dianci_judge_flag=0;
         Second_dianci_judge_flag=1;
     }

     if(flag_starting_line==1&&startline_flag==0)
     {
         bs_delay_time_flag=1;
         startline_flag=1;
     }


     else if(startline_flag==1&&flag_starting_line==1&&bs_stop_time_flag==1)
     {
         dianci_judge_flag=0;
         if(flag_left_starting_line==1)
         {
#if 1
             if(ad_right<10&&ad_middle<10&&ad_behind_left<10)
                        {
                            SpeedCtrl(0,0,0,0);
                        }
                        else
                        {
             SpeedCtrl(200,-250,200,-250);
                        }
#else
             SpeedCtrl(200,-250,200,-250);
#endif


         }
         else if(flag_right_starting_line==1)
         {
#if 1
            if(ad_right<10&&ad_middle<10&&ad_behind_left<10)   //入库停
            {
                SpeedCtrl(0,0,0,0);
            }
            else
            {
            SpeedCtrl(-250,200,-250,200);
            }
#else
            SpeedCtrl(-250,200,-250,200); //入库不停
#endif

         }
     }

 }


void Firstin_Delay_Time(void )

{
    if(firstin_delay_time_flag==1)
    {
        firstin_delay_time--;
        if(firstin_delay_time<=0)
        {

           Camera_Now_Angle=0;
            firstin_stop_time_flag=1;
        }
    }
 }


void Firstout_Delay_Time(void )

{
    if(firstout_delay_time_flag==1)
    {
        firstout_delay_time--;
        if(firstout_delay_time<=0)
        {
            firstout_stop_time_flag=1;
        }
    }
 }


void Secondin_Delay_Time(void)
{
    if(secin_delay_time_flag==1)
       {
           secin_delay_time--;
           if(secin_delay_time<=0)
           {
               secin_stop_time_flag=1;
           }
       }

    }

void Cancha_deal(void )
{

    if(dianci_judge_flag==1)
    {
        if(Now_Angle>(-75))   //三岔入口第一次自转
        {
        //    SpeedCtrl(100,-80,0,80);
       Dianci_Or_Camera_flag=2;
        pwm_duty(S_MOTOR_PIN,1100); //舵机旋转90度
        Now_Angle +=(gz)*Angle_T;
        }
//        else if(Now_Angle>(-85))
//        {
//            Dianci_Or_Camera_flag=6;//入岔第二阶段
//             pwm_duty(S_MOTOR_PIN,1100); //舵机旋转90度
//             Now_Angle +=(gz)*Angle_T;
//        }
        else
           {
               //dianci_judge_flag=0;
               Dianci_Or_Camera_flag=1;
               firstin_delay_time_flag=1;
               if(firstin_stop_time_flag==1)
               {
                   firstin_delay_time_flag=0;
                   if(RightLose>40&&LeftLose>35&&dianci_judge_flag==1&& HuanDao_flag==0)
                   {

                 //      dianci_judge_flag=0;
                       firstin_stop_time_flag=0;

                        Dianci_Or_Camera_flag=5;
                   }
                   if(ad_left>30&&ad_middle>30)
                   {

                       dianci_judge_flag=0;
                       firstin_stop_time_flag=0;
                       Dianci_Or_Camera_flag=0;
                   }
               }
         }
    }
 }

void Huan_Judge(uint16 mid,uint16 left_1,uint16 right_1)
{
    if(HuanDao_flag==0)
    {

    static  int16 last_value[9];    //创建一个数组队列
    static  int16 left_value[9];    //创建一个数组队列
    static  int16 right_value[9];   //创建一个数组队列
    uint8 i=0;
    uint8 x;
    int16 left_average=0;
    int16 right_average=0;

    for(i=0;i<8;i++)
    {
       last_value[i]=last_value[i+1];
    }
    last_value[8]=mid;

    for(i=0;i<8;i++)
    {
         left_value[i]=left_value[i+1];

     }
    left_value[8]=left_1;

    for(i=0;i<8;i++)
    {
         right_value[i]=right_value[i+1];

    }
   right_value[8]=right_1;


   left_average=(left_value[8]+left_value[7]+left_value[6]+left_value[5]+left_value[4]+left_value[3]+left_value[2]+left_value[1]+left_value[0])/9;
   right_average=(right_value[8]+right_value[7]+right_value[6]+right_value[5]+right_value[4]+right_value[3]+right_value[2]+right_value[1]+right_value[0])/9;


   if(((last_value[8]+last_value[7]+last_value[6]+last_value[5])/4)>((last_value[3]+last_value[2]+last_value[1]+last_value[0])/4))
   {
       if(((left_value[8]+left_value[7]+left_value[6]+left_value[5])/4)>((left_value[3]+left_value[2]+left_value[1]+left_value[0])/4))
        {
           if(((right_value[8]+right_value[7]+right_value[6]+right_value[5])/4)>((right_value[3]+right_value[2]+right_value[1]+right_value[0])/4))
             {

               if(((last_value[8]+last_value[7]+last_value[6]+last_value[5]+last_value[4]+last_value[3]+last_value[2]+last_value[1]+last_value[0])/9)>125)
               {
                   if(left_average>90)

                   {
                       if(right_average>90)
                      {
                          HuanDao_flag=1;

                          if(left_average>right_average)
                          {
                              Right_Or_Left_Huan_flag=1;
                              rt_mb_send(buzzer_mailbox, 300);//运用RT-Thread中的邮件的作用
                          }
                          else
                          {
                              rt_mb_send(buzzer_mailbox, 300);//运用RT-Thread中的邮件的作用，用于环岛的判断，使蜂鸣器间歇响应
                              Right_Or_Left_Huan_flag=2;
                          }
                      }
                   }
               }
             }
        }
      }
   }
}

void Huan_Deal(uint16 mid)
{

    static  int16 last_value[9];    //创建一个数组队列
    for(int16 i=0;i<8;i++)
      {
         last_value[i]=last_value[i+1];
      }
      last_value[8]=mid;

    if(Right_Or_Left_Huan_flag==2&&Dianci_Or_Camera_flag==0)//右环
    {

        if((((last_value[8]+last_value[7]+last_value[6]+last_value[5])/4)<((last_value[3]+last_value[2]+last_value[1]+last_value[0])/4))&&last_value[4]>120)
        {

            Dianci_Or_Camera_flag=3;
            Now_Angle=0;

        }
    }

    if(Right_Or_Left_Huan_flag==1&&Dianci_Or_Camera_flag==0)//右环
    {

        if((((last_value[8]+last_value[7]+last_value[6]+last_value[5])/4)<((last_value[3]+last_value[2]+last_value[1]+last_value[0])/4))&&last_value[4]>120)
        {

            Dianci_Or_Camera_flag=104;
            Now_Angle=0;

        }
    }

    }

void Search_speed_Control(void)
      {
      if(Dianci_Or_Camera_flag==1)
      {
          speedz_ctrl();
          Speed_Controler(-100,0,0,2); //开启横向摄像头循迹

      }
      else   if(Dianci_Or_Camera_flag==0)   //开启正常的电磁循迹
      {

          if(firstout_stop_time_flag==1)
          {
              firstout_delay_time_flag=0;
              firstout_stop_time_flag=0;
              HuanDao_flag=0;
          }
          pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度
          lowspeed_k_flag=0;
         highspeed_k_flag=1;
          stopspeed_k_flag=0;
          speedz_ctrl();

      Speed_Controler(0,120,speedz,1); //开启电磁循迹
      }
      else if(Dianci_Or_Camera_flag==2)   // 岔路车体自转
      {
          SpeedCtrl(100,-100,-100,100);
      }
      else if (Dianci_Or_Camera_flag==3)//进右环，直接差速
      {
          if(Now_Angle>(-20))
          {
              SpeedCtrl(100,-100,-100,100);
              Now_Angle +=(gz)*Angle_T;
          }
          else
          {
              Dianci_Or_Camera_flag=7;
          }

      }
      else if (Dianci_Or_Camera_flag==104)
      {
          if(Now_Angle<(30))
                   {
                       SpeedCtrl(-100,100,100,-100);
                       Now_Angle +=(gz)*Angle_T;
                   }
                   else
                   {
                       Dianci_Or_Camera_flag=105;
                   }

      }
      else if (Dianci_Or_Camera_flag==4)
      {
          Speed_Controler(0,120,0,3);            //摄像头循迹
      }
      else if (Dianci_Or_Camera_flag==5)
      {
          pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度

          if(Camera_Now_Angle<80)
          {
          SpeedCtrl(-120,120,120,-120);
          Camera_Now_Angle +=(gz)*Angle_T;
          }
          else
          {
              Dianci_Or_Camera_flag=0;

          }

      }
      else if (Dianci_Or_Camera_flag==6)//进岔路第二阶段
      {
          SpeedCtrl(100,-40,-40,100);
      }
      else if(Dianci_Or_Camera_flag==7)    //摄像头循迹
      {
          if(Now_Angle>(-100))
          {
              Now_Angle +=(gz)*Angle_T;
          Speed_Controler(0,120,0,3);            //摄像头循迹
          }
          else
          {
              Right_Or_Left_Huan_flag=0;
              Dianci_Or_Camera_flag=0;
              firstout_delay_time_flag=1;
          }

      }
      else if (Dianci_Or_Camera_flag==105)
      {
          if(Now_Angle<(100))
                   {
                       Now_Angle +=(gz)*Angle_T;
                   Speed_Controler(0,120,0,3);            //摄像头循迹
                   }
                   else
                   {
                       Right_Or_Left_Huan_flag=0;
                       Dianci_Or_Camera_flag=0;
                       firstout_delay_time_flag=1;
                   }
      }
      else if (Dianci_Or_Camera_flag==8)
      {
#if 1
          SpeedCtrl(160,-160,160,-160);
#else
          SpeedCtrl(-160,160,-160,160);

#endif
      }
      else if (Dianci_Or_Camera_flag==100)//第二次进三叉，舵机打角，进三叉
      {
          if(Second_In_Now_Angle>(-10))
              {
                  pwm_duty(S_MOTOR_PIN,1100); //舵机旋转90度
                  Second_In_Now_Angle +=(gz)*Angle_T;
                  SpeedCtrl(50,-50,-50,50);
              }
              else
              {
             Dianci_Or_Camera_flag=1;//第二次三岔路内开启摄像头横向循迹
              secin_delay_time_flag=1;
              }
      }
      else if(Dianci_Or_Camera_flag==101)  //第二次三岔路出，摄像头回正，车体回正
      {
          if(Now_Angle<50)
          {
          pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度
          SpeedCtrl(-100,100,100,-100);
          Now_Angle +=(gz)*Angle_T;
          }
          else
          {
              Dianci_Or_Camera_flag=0;   //开启正常长电磁寻线
          }
      }
      else if(Dianci_Or_Camera_flag==102)
      {
          if(Three_Now_Angle<110)
                  {
                  pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度
                  SpeedCtrl(-100,100,100,-100);
                  Three_Now_Angle +=(gz)*Angle_T;
                  }
                  else
                  {
                      Dianci_Or_Camera_flag=0;   //开启正常长电磁寻线
                  }
      }
      else
      {
          SpeedCtrl(0,0,0,0);
      }
 }


//Second_Dianci_Or_Camera_flag，第二次岔路控制，从100开始

void Second_Cancha_deal(void)
{
    if(Second_dianci_judge_flag==1&&dianci_judge_flag==1)

    {

      Dianci_Or_Camera_flag=100;// 第二次进三叉，

        if(secin_stop_time_flag==1)
        {
            if(RightLose>40&&LeftLose>35&&dianci_judge_flag==1&& HuanDao_flag==0)
                       {
                              secin_delay_time_flag=0;
                     //      dianci_judge_flag=0;
                           secin_stop_time_flag=0;
                           Second_dianci_judge_flag=0;
                            Dianci_Or_Camera_flag=101;
                       }
             if(ad_behind_left>=30)
                       {
                           secin_delay_time_flag=0;
                           Second_dianci_judge_flag=0;
                         //  dianci_judge_flag=0;
                           secin_stop_time_flag=0;
                           Dianci_Or_Camera_flag=102;
                       }
              }

        }

}
/*
void Second_Search_speed_Control(void)
{
    if(Second_Dianci_Or_Camera_flag==0)
    {
        pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度
            lowspeed_k_flag=1;
           highspeed_k_flag=0;
            stopspeed_k_flag=0;
            speedz_ctrl();
        Speed_Controler(0,160,speedz,1); //开启电磁循迹
    }
    else  if(Second_Dianci_Or_Camera_flag==1&&Second_Dianci_Or_Camera_flag==1)
    {

        if(Second_In_Now_Angle>(-10))
        {
            pwm_duty(S_MOTOR_PIN,1100); //舵机旋转90度
            Second_In_Now_Angle +=(gz)*Angle_T;
            SpeedCtrl(50,-50,-50,50);

        }
        else
        {

        Speed_Controler(-120,0,0,2);

    //    Speed_Controler(120,0,0,2);
        secin_delay_time_flag=1;
        }
    }

    else  if(Second_Dianci_Or_Camera_flag==2)
     {
        if(Now_Angle<30)
        {
        pwm_duty(S_MOTOR_PIN,640); //舵机旋转90度
        SpeedCtrl(-100,100,100,-100);
        Now_Angle +=(gz)*Angle_T;
        }
    }
           else if(Second_Dianci_Or_Camera_flag==255)
    {
               Second_Dianci_Or_Camera_flag=0;
    }

    }

*/



