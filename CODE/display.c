
#include "display.h"


extern uint8 ad_left;
extern uint8 ad_right;
extern uint8 ad_middle;
extern uint8 ad_behind_left;

/* 四个轮子的实际速度  */
extern    int16  realSpeed1 ;
extern    int16 realSpeed2 ;
extern    int16 realSpeed3 ;
extern    int16 realSpeed4 ;


void display_entry(void *parameter)
{


    while(1)
    {
     //   ips114_displayimage032(Image[0], MT9V03X_W, MT9V03X_H);


        ips114_showint16(120,0, ad_left);//adc_mean_filter(ADC_IN4_A4, ADC_8BIT,5));
          ips114_showint16(120,1, ad_right);//adc_mean_filter(ADC_IN6_A6, ADC_8BIT,5));
          ips114_showint16(120,2, ad_middle);//adc_mean_filter(ADC_IN8_B0, ADC_8BIT,5));
          ips114_showint16(120,3, ad_behind_left);//adc_mean_filter(ADC_IN9_B1, ADC_8BIT,5));

          ips114_showint16(170,0,realSpeed1);
          ips114_showint16(170,1,realSpeed2);
          ips114_showint16(170,2,realSpeed3);
          ips114_showint16(170,3,realSpeed4);

          //   ips114_showint16(120,4,icm_gyro_z);
       //   ips114_showint16 (170,4, Speed_Acl);
       //   ips114_showfloat  (170,5,Now_Angle,2,3);

    }
    
}





void display_init(void)
{
    rt_thread_t tid1;

    //初始化屏幕
    ips114_init();
    
    //创建显示线程 优先级设置为6 
    tid1 = rt_thread_create("display", display_entry, RT_NULL, 1024, 6, 50);
    
    //启动显示线程
    if(RT_NULL != tid1)
    {
        rt_thread_startup(tid1);
    }
}
