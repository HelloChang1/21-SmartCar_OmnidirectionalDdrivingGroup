#include "encoder.h"

#define ENCODER1_TIM		TIM_1
#define ENCODER1_A			TIM_1_ENC1_A08
#define ENCODER1_B			TIM_1_ENC2_A01

#define ENCODER2_TIM		TIM_8
#define ENCODER2_A			TIM_8_ENC1_C00
#define ENCODER2_B			TIM_8_ENC2_C01


    /* 四个轮子的实际速度  */
extern    int16  realSpeed1 ;
extern    int16 realSpeed2 ;
extern    int16 realSpeed3 ;
extern    int16 realSpeed4 ;

void EncInit(void)
{
    //这里的编码器是绝对式编码器，也称角度编码器。
    encoder_init_spi(ABS_ENCODER_SPI_PC4_PIN);      //编码器1初始化。
    encoder_init_spi(ABS_ENCODER_SPI_PC3_PIN);      //编码器2初始化。
    encoder_init_spi(ABS_ENCODER_SPI_PC1_PIN);      //编码器3初始化。
    encoder_init_spi(ABS_ENCODER_SPI_PC2_PIN);      //编码器4初始化。
}

void SpeedGet(void)
{

   realSpeed1 = encoder1_speed_spi(ABS_ENCODER_SPI_PC4_PIN);
    realSpeed2 = encoder2_speed_spi(ABS_ENCODER_SPI_PC3_PIN);
    realSpeed3 = encoder3_speed_spi(ABS_ENCODER_SPI_PC1_PIN);
    realSpeed4 = encoder4_speed_spi(ABS_ENCODER_SPI_PC2_PIN);
}
