

//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新

#include "headfile.h"

#define S_MOTOR_PIN PWM2_CH1_A0 //定义舵机引脚

int main(void) {

  mt9v03x_init();                 //摄像头初始化
  icm20602_init_spi();            //六轴陀螺仪加速计初始化
  motor_init();                   //电机初始化
  EncInit();                      //编码器初始化
  elec_init();                    //电感初始化
  buzzer_init();                  //蜂鸣器初始化
  button_init();                  //按键初始化
  display_init();                 //液晶显示初始化
  pwm_init(S_MOTOR_PIN, 50, 640); //舵机初始化//670-1400,640-1100
  SensorData_Threads_Init();      //数据采集多线程初始化
  timer_pit_init();               // 定时器周期运动控制初始化
}
