#include "elec.h"

uint8 ad_left;
uint8 ad_right;
uint8 ad_middle;
uint8 ad_behind_left;


void elec_init(void)
{
    adc_init(ADC_IN4_A4);   //��ų�ʼ��
     adc_init(ADC_IN6_A6);
     adc_init(ADC_IN9_B1);
     adc_init(ADC_IN8_B0);
}


void elec_get(void)
{
    ad_left = adc_mean_filter(ADC_IN4_A4, ADC_8BIT,5);//�ɼ�������ֵ �ɼ�5����ƽ��
    ad_right = adc_mean_filter(ADC_IN6_A6, ADC_8BIT,5);//�ɼ��ҵ����ֵ �ɼ�5����ƽ��
    ad_middle=adc_mean_filter(ADC_IN8_B0, ADC_8BIT,5);   //���µ�вɼ�ֵ
    ad_behind_left=adc_mean_filter(ADC_IN9_B1, ADC_8BIT,5); //���µ�вɼ�ֵ
    
}

void elec_calculate(void)
{
    
}
