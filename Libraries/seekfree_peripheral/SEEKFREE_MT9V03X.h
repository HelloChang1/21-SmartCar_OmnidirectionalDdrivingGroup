/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		总钻风
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 * @note		
					接线定义：
					------------------------------------ 
						模块管脚                          单片机管脚
						SDA(51的RX)         查看SEEKFREE_MT9V03X.h文件中的MT9V03X_COF_UART_TX宏定义
						SCL(51的TX)         查看SEEKFREE_MT9V03X.h文件中的MT9V03X_COF_UART_RX宏定义
						场中断(VSY)          查看SEEKFREE_MT9V03X.h文件中的MT9V03X_VSYNC_PIN宏定义
						行中断(HREF)		        不接
						像素中断(PCLK)       查看SEEKFREE_MT9V03X.h文件中的MT9V03X_PCLK_PIN宏定义
						数据口(D0-D7)		查看SEEKFREE_MT9V03X.h文件中的MT9V03X_DATA_PIN宏定义
					------------------------------------ 
	
                                                默认分辨率是            188*50
                                                默认FPS                 50帧
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V03X_h
#define _SEEKFREE_MT9V03X_h

#include "common.h"
#include "zf_uart.h"
#include "ch32v10x.h"

//配置摄像头参数

#define MT9V03X_W               120            //图像宽度  范围1-188
#define MT9V03X_H               60              //图像高度  推荐使用50



//--------------------------------------------------------------------------------------------------
//引脚配置
//--------------------------------------------------------------------------------------------------
#define MT9V03X_COF_UART        UART_2              //配置摄像头所使用到的串口
#define MT9V03X_COF_UART_TX     UART2_TX_A2
#define MT9V03X_COF_UART_RX     UART2_RX_A3

#define MT9V03X_VSYNC_PIN       A1                  //场中断引脚
#define MT9V03X_VSYNC_IRQN      EXTI1_IRQn          //中断号

#define MT9V03X_DMA_CH          DMA1_Channel4
#define MT9V03X_DMA_IRQN        DMA1_Channel4_IRQn

#define MT9V03X_PCLK_PIN        A12                 //GPIO触发TIM引脚禁止随意修改
//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        GPIO_IDR_ADDR(2,0)
//        第一个参数2代表着使用C口，可以选择0、1、2、3分别对应GPIO_PIN(A,0)，GPIO_PIN(B,0)，GPIO_PIN(C,0)，GPIO_PIN(D,0)
//        第二个参数0代表着使用的是C0 - C7口,
//        可以选择0、1、2、3，分别对应的是    该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是C0-C7)
//--------------------------------------------------------------------------------------------------
#define MT9V03X_DATA_PIN        GPIO_IDR_ADDR(2, 0)


                        
//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数
    
    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令
	
    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;
     
     
     
extern uint8    receive[3];         //仅用于接收配置信息时所用  禁止用户使用该变量
extern uint8    receive_num;        //仅用于接收配置信息时所用  禁止用户使用该变量
extern vuint8   uart_receive_flag;  //仅用于接收配置信息时所用  禁止用户使用该变量


extern vuint8   mt9v03x_finish_flag;//一场图像采集完成标志位
extern uint8    Image[MT9V03X_H][MT9V03X_W];


void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
uint16 get_version(UARTN_enum uartn);
uint16 set_exposure_time(UARTN_enum uartn, uint16 light);

void mt9v03x_init(void);
void mt9v03x_vsync(void);
void mt9v03x_dma(void);
void mt9v03x_uart_callback(void);
void seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);




#endif

