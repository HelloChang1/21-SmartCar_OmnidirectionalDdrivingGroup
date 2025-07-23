/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            zf_uart
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/

#include "ch32v10x_rcc.h"
#include "ch32v10x_gpio.h"
#include "ch32v10x_usart.h"
#include "ch32v10x_misc.h"
#include "zf_gpio.h"
#include "zf_uart.h"
#include "zf_nvic.h"


const uint32 UARTN[] = {USART1_BASE, USART2_BASE, USART3_BASE ,UART4_BASE};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口引脚初始化
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void uart_gpio_init(UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    if(tx_pin == UART1_TX_A9 && rx_pin == UART1_RX_A10)
    {
        gpio_init(A9, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(A10, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART1_TX_B6)
    {
        gpio_init(B6, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(B7, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART2_TX_A2)
    {
        gpio_init(A2, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(A3, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_B10)
    {
        gpio_init(B10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(B11, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_C10)
    {
        gpio_init(C10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
        gpio_init(C11, GPI, 0, IN_FLOAT);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      uartn       串口通道
//  @param      baud        波特率
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10); //串口1初始化引脚号,TX为A9,RX为A10
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    USART_InitTypeDef USART_InitStructure;
    USART_DeInit((USART_TypeDef*)UARTN[uartn]);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //引脚初始化
    uart_gpio_init(tx_pin, rx_pin);

    if(UART1_TX_B6 == tx_pin && UART1_RX_B7 == rx_pin)                         //引脚重映射
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    if(UART3_TX_C10 == tx_pin && UART3_RX_C11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

    //串口总线使能
    if(UART_1 == uartn) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(UART_2 == uartn) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(UART_3 == uartn) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //串口参数配置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //使能串口
    USART_Init((USART_TypeDef*)UARTN[uartn], &USART_InitStructure);
    USART_Cmd((USART_TypeDef*)UARTN[uartn], ENABLE);


}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn       串口通道
//  @param      dat         串口数据
//  @return     void
//  Sample usage:           uart_putchar(UART_1, 0x43);        //串口1发送0x43。
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTN_enum uartn, uint8 dat)
{
    while((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_TXE)==0);
    ((USART_TypeDef*)UARTN[uartn])->DATAR = dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn       串口通道
//  @param      buff        要发送的数组地址
//  @param      len         数据长度
//  @return     void
//  Sample usage:           uart_putbuff(UART_1, buff, 10);     //串口1发送10个buff数组。
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(UARTN_enum uartn, uint8 *buff, uint32 len)
{
    while(len--)
        uart_putchar(uartn, *buff++);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn       串口通道
//  @param      str         字符串首地址
//  @return     void
//  Sample usage:           uart_putstr(UART_1, (uint8 *)"12345")   //串口1发送12345这个字符串
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(UARTN_enum uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口接收中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_rx_irq(UART_1, ENABLE);        //打开串口1接收中断
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq(UARTN_enum uartn,uint8 status)
{
    USART_ITConfig(((USART_TypeDef*)UARTN[uartn]), USART_IT_RXNE, ENABLE);
    nvic_init(USART1_IRQn + uartn, 1, 0, status);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口发送中断
//  @param      uartn       串口通道
//  @param      status      使能或者失能
//  @return     void
//  Sample usage:           uart_tx_irq(UART_1, DISABLE);       //关闭串口1发送 中断
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_irq(UARTN_enum uartn,uint8 status)
{
    USART_ITConfig(((USART_TypeDef*)UARTN[uartn]), USART_IT_TXE, ENABLE);
    nvic_init(USART1_IRQn + uartn, 1, 0, status);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（whlie等待）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     void
//  Sample usage:               uint8 dat; uart_getchar(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar(UARTN_enum uartn, uint8 *dat)
{
    while((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_RXNE) == 0);
    *dat = (uint16_t)(((USART_TypeDef*)UARTN[uartn])->DATAR & (uint16_t)0x01FF);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      uartn           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     uint8           1：接收成功   0：未接收到数据
//  Sample usage:               uint8 dat; uart_query(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UARTN_enum uartn, uint8 *dat)
{
    if((((USART_TypeDef*)UARTN[uartn])->STATR & USART_FLAG_RXNE) != 0)
    {
        *dat = (uint16_t)(((USART_TypeDef*)UARTN[uartn])->DATAR & (uint16_t)0x01FF);
        return 1;
    }
    return 0;
}





    
    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      对摄像头内部寄存器进行写操作
//  @param      uartn       选择使用的串口
//  @param      addr        摄像头内部寄存器地址
//  @param      data        需要写入的数据
//  @return     uint16      寄存器当前数据，用于确认是否写入成功
//  @since      v1.0
//  Sample usage:           调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v03x_reg(UARTN_enum uartn, uint8 addr, uint16 data)
{
    uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn,send_buffer,4);
    systick_delay_ms(10);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn,send_buffer,4);
    
    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;

}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风场中断初始化
//  @param      void        NULL
//  @return     void        NULL
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_vsync_init(void)
{
    gpio_interrupt_init(MT9V03X_VSYNC_PIN, FALLING, IN_PULLUP);
    nvic_init(MT9V03X_VSYNC_IRQN, 1, 0, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风场GPIO触发定时器初始化
//  @param      void        NULL
//  @return     void        NULL
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_tim1_etr_init(void)
{
    camera_tim_etr_init(TIMER_1, MT9V03X_PCLK_PIN);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风DMA初始化
//  @param      void        NULL
//  @return     void        NULL
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_dma_init(void)
{
    uint8 data_pin,num;


    //摄像头数据引脚初始化
    data_pin = (uint8)(((MT9V03X_DATA_PIN - (GPIOA_BASE+8)) >> 10) << 5) +
                (((MT9V03X_DATA_PIN - (GPIOA_BASE+8)) & 0x0F) << 3);

    for(num=7; num>0; num--)
    {
        gpio_init((PIN_enum)(num + data_pin), GPI, 0, IN_FLOAT);
    }


    //DMA摄像头初始化
    camera_dma_init(MT9V03X_DMA_CH,(uint32)MT9V03X_DATA_PIN, (uint32)camera_buffer_addr, MT9V03X_H*MT9V03X_W);
    nvic_init(MT9V03X_DMA_IRQN, 0, 0, ENABLE);          //中断配置
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X摄像头初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:   使用FLEXIO接口采集摄像头
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_init(void)
{
    
    camera_type = CAMERA_GRAYSCALE;//设置连接摄像头类型
    camera_buffer_addr = Image[0];

    uart_init (MT9V03X_COF_UART, 9600,MT9V03X_COF_UART_TX, MT9V03X_COF_UART_RX);    //初始换串口 配置摄像头
    uart_rx_irq(MT9V03X_COF_UART, ENABLE);
    //EnableGlobalIRQ(0);
    uart_receive_flag = 0;
    //等待摄像头上电初始化成功 方式有两种：延时或者通过获取配置的方式 二选一
    //systick_delay_ms(1000);//延时方式
    get_config(MT9V03X_COF_UART, GET_CFG);//获取配置的方式

    uart_receive_flag = 0;
    set_config(MT9V03X_COF_UART, MT9V03X_CFG);

    uart_receive_flag = 0;
    //获取配置便于查看配置是否正确
    get_config(MT9V03X_COF_UART, GET_CFG);
    
    //DisableGlobalIRQ();

    //DMA初始化
    mt9v03x_dma_init();
    //GPIO触发定时器初始化,PLCK引脚初始化
    mt9v03x_tim1_etr_init();
    //VSYNC初始化
    mt9v03x_vsync_init();
}


vuint8   mt9v03x_finish_flag = 0;    //一场图像采集完成标志位
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_vsync(void)
{
    MT9V03X_DMA_CH->CNTR = MT9V03X_H*MT9V03X_W;             //设置当前DMA传输的剩余数量，向下递减，该寄存器只能在DMA不工作时更改。
    MT9V03X_DMA_CH->CFGR |= DMA_CFGR1_EN;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X摄像头DMA完成中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
extern rt_sem_t camera_sem;
void mt9v03x_dma(void)
{
    mt9v03x_finish_flag = 1;//一副图像从采集开始到采集结束耗时3.8MS左右(50FPS、188*120分辨率)
    MT9V03X_DMA_CH->CFGR &= (uint16)(~DMA_CFGR1_EN);   //关闭DMA1
    rt_sem_release(camera_sem);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      uartn           使用的串口号
//  @param      image           需要发送的图像地址
//  @param      width           图像的列
//  @param      height          图像的行
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn,0x00);uart_putchar(uartn,0xff);uart_putchar(uartn,0x01);uart_putchar(uartn,0x01);//发送命令
    uart_putbuff(uartn, image, width*height);  //发送图像
}



