#include "elec.h"

uint8 ad_left;
uint8 ad_right;
uint8 ad_middle;
uint8 ad_behind_left;


void elec_init(void)
{
    adc_init(ADC_IN4_A4);   //电磁初始化
     adc_init(ADC_IN6_A6);
     adc_init(ADC_IN9_B1);
     adc_init(ADC_IN8_B0);
}


void elec_get(void)
{
    ad_left = adc_mean_filter(ADC_IN4_A4, ADC_8BIT,5);//采集左电感数值 采集5次求平均
    ad_right = adc_mean_filter(ADC_IN6_A6, ADC_8BIT,5);//采集右电感数值 采集5次求平均
    ad_middle=adc_mean_filter(ADC_IN8_B0, ADC_8BIT,5);   //左下电感采集值
    ad_behind_left=adc_mean_filter(ADC_IN9_B1, ADC_8BIT,5); //右下电感采集值
    
}

void elec_calculate(void)
{
    
}
