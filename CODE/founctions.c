
#include "functions.h"



uint8 left_down_duandian=MT9V03X_H,right_down_duandian=MT9V03X_H;
uint8 left_duandian_up=0,right_duandian_up=0;

float t_a,t_b,t_c;


uint8 sqrt1(uint8 n){
 int left=0;
 int right=n;
 int mid;
 int last;
 while(left<=right){//应该找出mid*mid<=n的最后一个数
  mid=(right-left)/2+left;

  if(mid*mid<=n){//寻找最后一个数，所以不断压缩左边，即left=mid+1
   last=mid;
   left=mid+1;
  }else{
   right=mid-1;
  }
 }
return last;
}


int My_abs(int num)
{
    if(num<0)
        num=-num;
    return num;
}

/*曲率的计算*/
float curvature(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3)
{
    float k;
    int area = (int)((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
    uint8 q1=(uint8)((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    uint8 AB=(uint8)sqrt1(q1);
    q1=(uint8)((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
    uint8 BC=(uint8)sqrt1(q1);
    q1=(uint8)((x3-x1)*(x3-x1)+(y3-y1)*(y3-y1));
    uint8 AC=(uint8)sqrt1(q1);
    if(AB*AC*BC==0)k=0;
    else
        k=(float)4*area/(AB*AC*BC);
    return k;
}



