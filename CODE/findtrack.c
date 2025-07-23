
#include "findtrack.h"

#define flaw  0
uint8 l_start=58,r_start=58,break_row;
uint8 mid;
uint8 Line=60;
uint8 r_start_r,l_start_l;
/*********define for SearchCenterBlackline**********/

int   MiddleLine[RowMax+1];
int   RightEdge[RowMax+1];
int   LeftEdge[RowMax+1];
int   Width[RowMax+1];





int   MidPri         = MT9V03X_W/2; // 图像的中间位置
int   LastLine       = 0;  // 上一行 
float AvaliableLines = 0;  // 有效行数 
int   LeftLose       = 0;  // 左边线丢失的数目 
int   RightLose      = 0;  // 右边线丢失的数目 
int   AllLose        = 0;  // 左右边线都丢失的数目 
int   LeftLoseStart  = 0;  // 记录左边丢线的开始行
int   RightLoseStart = 0;  // 记录右边边丢线的开始行
int   WhiteStart     = 0;  // 空白行的起点 
int LastLastLine     = 0;  // 上上一行 




//设置中线，左线，右线的初始化值
//设置每一行对应的赛道宽度
//本文件内不调用
#define Width_P  1.55//0.95
void SetInitVal()
{
  int i;
  
  for(i=0;i<RowMax;i++)//赋初值             
  { 
      RightEdge[i]  = ColumnMax-flaw;  //右边界位置
      LeftEdge[i]   = flaw;          //左边界位置
      MiddleLine[i] = ColumnMax/2;         //中线的位置初始化
      Width[i]  =(int)(16+i*Width_P); //动态路宽                          ??????
      if(Width[i] >=ColumnMax-3)
      {
        Width[i]=ColumnMax-3;                  //最大路宽限幅
      }
  }
  
} 


int MAX(int m, int n)
{
    if (m>n)
    {
        return m;
    }
    else if (m<n)
    {
        return n;
    }
    else
    {
        return n;

    }




}

void judge_eg()
{
    int x,y,m;
    int flag_up_left_duandian=0,flag_up_right_duandian=0;
    uint8 mid;
    uint8 leftflag,rightflag;

    LeftLose        = 0;//变量清零
    RightLose       = 0;
    AllLose         = 0;
    Line            = 60;
    r_start         =MT9V03X_H;
    l_start         =MT9V03X_H;

    break_row=0;

    mid=MT9V03X_W/2;
    if(Image[MT9V03X_H][mid]==Black_Point||LeftLose>MT9V03X_H-1)//车往右边偏
    {
        for(x=mid;x>0;x--)
        {
            if(Image[MT9V03X_H][x]==Black_Point&&Image[MT9V03X_H][x+1]==Black_Point&&Image[MT9V03X_H][x+2]==White_Point)
            {
                mid=x-8;
                if(mid<0)
                    mid=5;
            }
        }
    }
    if(Image[MT9V03X_H][mid]==Black_Point||RightLose>MT9V03X_H-1)//车往左边偏
    {
        for(x=mid;x<MT9V03X_W;x++)
        {
            if(Image[MT9V03X_H][x]==Black_Point&&Image[MT9V03X_H][x-1]==Black_Point&&Image[MT9V03X_H][x-2]==White_Point)
            {
                mid=x+8;
                if(mid>MT9V03X_W)
                    mid=MT9V03X_W-5;
            }
        }
    }
    for(y=MT9V03X_H;y>(MT9V03X_H-Line);y--)
    {
       /*左边界判断*/
        leftflag=0;

        for(x=mid;x>2;x--)
        {

            if(Image[y][x]==White_Point&&Image[y][x-1]==0&&Image[y][x-2]==0){
                leftflag=1;
                LeftEdge[y]=x;
                        break;
            }
        }
        /*右边界判断*/

        rightflag=0;
        for(x=mid;x<MT9V03X_W-3;x++)
        {

            if(Image[y][x]==White_Point&&Image[y][x+1]==0&&Image[y][x+2]==0){
                rightflag=1;
                RightEdge[y]=x;
                    break;
            }
        }

        if((LeftEdge[y]-LeftEdge[y-1])>19&&(LeftEdge[y]-LeftEdge[y-2]>19))left_down_duandian=y;//左线断点位置
        if((RightEdge[y-1]-RightEdge[y])>19&&(RightEdge[y-2]-RightEdge[y])>19)right_down_duandian=y;//右线断点位置
        if((LeftEdge[y+1]-LeftEdge[y+2])>19&&(LeftEdge[y]-LeftEdge[y+2]>19)&&flag_up_left_duandian==0){left_duandian_up=y+2;flag_up_left_duandian=1;}//左线断点位置
        if((RightEdge[y+1]-RightEdge[y+2])>19&&(RightEdge[y]-RightEdge[y+2])>19&&flag_up_right_duandian==0){right_duandian_up=y+2;flag_up_right_duandian=1;}//右线断点位置
        if(leftflag==0)
            {
                LeftEdge[y]=2;
                LeftLose++;//左边线丢失次数
                if(LeftEdge[y+1]==2&&LeftEdge[y+2]!=2&&LeftEdge[y+3]!=2)
                    l_start=y+1;
            }

        if(rightflag==0)
            {
                RightEdge[y]=MT9V03X_W-2;
                RightLose++;//右边线丢失次数
                if(RightEdge[y+1]==MT9V03X_W-2&&RightEdge[y+2]!=MT9V03X_W-2&&RightEdge[y+3]!=MT9V03X_W-2)
                    r_start=y+1;
//                if(y>MT9V03X_H/2)
//                {
//                    if(RightEdge[y]!=MT9V03X_W-2&&RightEdge[y+1]==MT9V03X_W-2&&RightEdge[y+2]==MT9V03X_W-2)
//                        r_start_r=y;
//                }
            }

        if(rightflag==0&&leftflag==1)//右边界丢失
        {
            MiddleLine[y]=(uint8)(LeftEdge[y]+Width[y]/2);
            if(My_abs(RightEdge[y]-MiddleLine[y])<10)         //截至行
            {
                MiddleLine[y]=(uint8)(RightEdge[y]+LeftEdge[y])/2;
                break_row++;
//                break;
            }
        }
        else if(rightflag==1&&leftflag==0)//左边界丢失
        {
            MiddleLine[y]=(uint8)(RightEdge[y]-Width[y]/2);
            if(My_abs(MiddleLine[y]-LeftEdge[y])<10)               //截至行
            {
                MiddleLine[y]=(uint8)(RightEdge[y]+LeftEdge[y])/2;
                break_row++;
//                break;
            }
        }
        else if(rightflag==0&&leftflag==0)//右边界丢失
        {
            AllLose++;
            if(y==MT9V03X_H)
                MiddleLine[y]=MT9V03X_H/2;
            else
            MiddleLine[y]=MiddleLine[y+1];
        }
        else if((rightflag==1&&leftflag==1)||(break_row>10))
        {
            MiddleLine[y]=(uint8)(RightEdge[y]+LeftEdge[y])/2;   //中线计算
        }

        if(y == 0)
           {
               AvaliableLines = MT9V03X_H;
               LastLine  = 0;

           }
           m = MiddleLine[y] ;
           if(m < 5)// 如果中线的位置小于第5列
           {
               m = 5;
           }
           if(m > MT9V03X_W-5)
           {
               m = MT9V03X_W-5;// 如果中线的位置大于第75列
           }
           if( (LeftEdge[y]!=2 && LeftEdge[y]>=MT9V03X_W-5) || (RightEdge[y]!=ColumnMax-2 && RightEdge[y]<5) || ((y>=4)&&(Image[y-1][m]== Black_Point)&&(Image[y-2][m]== Black_Point))) //最后一行
           {
               LastLine = y;//最后一行，动态前瞻
               AvaliableLines = MT9V03X_H - y;//有效行数

           }
        mid=MiddleLine[y+1];
    }


// 一场图像扫描完毕

//以下是记录实际图像的路宽

}


       

