
#include "elements.h"

uint8 flow;
int16 total=0;
uint8 flag_enter_starting_line,flag_left_starting_line,flag_right_starting_line;
uint8 flag_starting_line;
uint8 Fork_Pre_flag,Fork_flag;
uint8 zhidao_flag;
uint8 l_bend_flag;
uint8 r_bend_flag;
int sum_cross=0;

void check_starting_line()
{ //int[] black_nums_stack = new int[20];
    uint8 times = 0;
    int start,end;
    {
        for (uint8 y = MT9V03X_H-2; y > 10; y--)
        {
            uint8 black_blocks = 0;
            uint8 cursor = 0;    //指向栈顶的游标
            for (uint8 x = LeftEdge[MT9V03X_H-2]+1; x <= RightEdge[MT9V03X_H-2]-1; x++)
            {
                if (Image[y][x] == 0)
                {
                    cursor++;
                }
                else
                {
                    if (cursor >= 4 && cursor <= 8)
                    {
                        black_blocks++;
                        cursor = 0;
                    }
                    else
                    {
                        cursor = 0;
                    }
                }
            }
            if (black_blocks >= 4 && black_blocks <= 16) times++;
            if(black_blocks==1)start=y;

//            lcd_showint32(110,10,start,5);
        }
        if (times >= 3 && times <= 30)
        {
            flag_starting_line = 1;
//            BuzzerRing;

        }
        else
            flag_starting_line=0;

        if(flag_starting_line==1)
        {
            if(LeftLose<3&&RightLose>20)
            {
                flag_left_starting_line=1;
                flag_starting_line=0;
            }
            else if(RightLose<3&&LeftLose>20)
            {
                flag_right_starting_line=1;
                flag_starting_line=0;
            }
        }
    }
}

extern uint8 dianci_judge_flag;


void chalupanduan()
{
    if(dianci_judge_flag==1)
    {

    uint8 chazhi1[RowMax];

    int x1=0,y1=0,x2=0,y2=0;
    int x,y;
    int temp = 0;
    int j;
    int min1=0,max1=0,max=0,min2=0,max2=0;
    int demp = 0;
    int start=0;
    int mid_piont_y,mid_piont_x;
    float BC,Wr;
     for(x=55;x>10;x--)
     {
        //if(center[x]==0)
        chazhi1[x]=RightEdge[x]-LeftEdge[x];//左边横坐标的数值-右边横坐标的数值
//                hengchazhi[x]=LeftEdge[0][x]-RightEdge[0][x];
     }
//            max=hengchazhi[0];
     for(int i=50;i>10;i--)     //  for(int i=20;i<50;i++)
    {
        if(chazhi1[i] > chazhi1[i+1])  //   if(chazhi1[i] < chazhi1[i+1])
           temp++;
        if(chazhi1[i] < chazhi1[i+1])  //   if(chazhi1[i] < chazhi1[i+1])
           demp++;
        if(temp<3)
        {
            start=i;
        }
    }
//     ips114_showint32(130,0,start,5);
     sum_cross=0;
     if(r_start>l_start)max=r_start;
     else
         max=l_start;
     mid_piont_y=0;
     for(int y=MT9V03X_H-2;y>10;y--)
     {
         for(int x=2;x<MT9V03X_W;x++)
         {
             if(Image[y][x]==White_Point&&Image[y-1][x]==0&&Image[y-2][x]==0)
             {
                 if(Image[y+1][x]==White_Point&&Image[y+2][x]==White_Point&&Image[y+3][x]==White_Point)
                     {
                         mid_piont_y=y;
                         mid_piont_x=x;
                         break;
                     }

             }

         }
         if(mid_piont_y!=0)break;
     }


         for(int x=mid_piont_x;x<mid_piont_x+10;x++)
         {
             for(int y=mid_piont_y+10;y>mid_piont_y-5;y--)
             {
                 if(Image[y][x]==White_Point&&Image[y-1][x]==0&&Image[y-2][x]==0)
                     {max1++;break;}
             }
             if(max1==10)
             {y1=y;x1=x;}
         }
         for(int x=mid_piont_x;x>mid_piont_x-10;x--)
             {
                 for(int y=mid_piont_y+10;y>mid_piont_y-5;y--)
                 {
                     if(Image[y][x]==White_Point&&Image[y-1][x]==0&&Image[y-2][x]==0)
                         {min1++;break;}
                 }
                 if(min1==10)
                 {x2=x;y2=y;}
             }


         for(int y=10;y>5;y--)
         {
             for(int x=mid_piont_x;x>2;x--)
             {
                 if(Image[y][x]==White_Point)
                 {min2++;break;}
             }
             for(int x=mid_piont_x;x<118;x++)
               {
                   if(Image[y][x]==White_Point)
                   {max2++;break;}
               }
         }


         BC=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
         Wr=(max1*max1+min1*min1-BC)/(2*max1*min1);

         if(max1==10&&min1==10&&Wr<-0.5&&Wr>-1&&max2==0&&min2==0)
             flow++;


     if(temp>8&&(LeftLose<25&&RightLose<25))
     {
         flow++;
      }
     if(temp>8&&LeftLose<20&&RightLose>20)
      {

         flow++;
      }
 else if(temp>8&&RightLose<20&&LeftLose>20)
         {
     flow++;
         }

     if(flow>2)
     {
         flow=0;
         Fork_Pre_flag=1;
     }

         if(Fork_Pre_flag==1&&mid_piont_y>50)
         {
             Fork_flag=1;
//             Fork_Pre_flag=0;
          }
    }
  }

//}
int16  pianchazhi()
{
    int16 error_acl=0;
     for (int i=MT9V03X_H-2;i>8;i--)
     {
         error_acl+=MiddleLine[i]-60;//MiddleLine[i-1];//MiddleLine[40];

  // 0-25，160，-80，
     }
     return error_acl;
}



/***********************************弯道判断*****************************************/

/*中线提取出来后，总共有40行的图像，运用前三十行进行斜率的计算，根据斜率的加权平均判断直弯道。
也可以根据不同段的斜率判断车辆所处的位置。*/
//如果中线不连续有没有影响

void WD_judge(void )
{
    int zhidao_flag;
    float slop1,slop2,slop3,cur;
    int16 chazhi,x;

    chazhi=pianchazhi();

    cur = curvature(MiddleLine[MT9V03X_H-5],MT9V03X_H-5,MiddleLine[MT9V03X_H-20],MT9V03X_H-20,MiddleLine[MT9V03X_H-45],MT9V03X_H-45);

    if(cur>-0.1&&cur<0.2)
    {
        if(((total<12)&&(chazhi>-900))||((total>-13)&&(chazhi<930)))
        {
            zhidao_flag=1;
            r_bend_flag=0;
            l_bend_flag=0;
            for (int y =MT9V03X_H; y>LastLine ; y--)
            {
                if(LeftEdge[y]!=0 && RightEdge[y]<=MT9V03X_W-2)
                Width[y]=RightEdge[y]-LeftEdge[y];
                else if(LeftEdge[y]==0 && RightEdge[y]<=MT9V03X_W-2)  //丢左线 右线不丢
                    Width[y]=RightEdge[y];
                else if(LeftEdge[y]!=0 && RightEdge[y]>=MT9V03X_W-2)  //丢右线 左线不丢
                    Width[y]=MT9V03X_W-LeftEdge[y];
                else Width[y]=MT9V03X_W-2;

            }
//            BuzzerRing;
        }
    }
    /*理论上只适合于90度弯，不知实际效果怎么样*/
    /*分为高速、低速、中速、高速区*/
    //左弯
    if(cur<-0.1)
    {
        if(total>15&&chazhi<-210&&Fork_Pre_flag==0)
        {
            l_bend_flag=1;
            zhidao_flag=0;
//            Fork_Pre_flag=0;
        }
    }

//    //右弯
    if(cur>0.12)
    {
        if(total<-15&&(chazhi>150||chazhi<-100)&&Fork_Pre_flag==0)
        {
            r_bend_flag=1;
            zhidao_flag=0;
//            Fork_Pre_flag=0;
        }
    }

}



