/*
 * CSM_camera_deal.c
 *
 *  Created on: 2021年5月14日
 *      Author: Administrator
 */

#include "CSM_camera_deal.h"

uint8 GetOSTU_v2(unsigned char *image) {
  unsigned char *np;        // 图像指针
  uint8 thresholdValue = 1; // 阈值

  int ihist[256] = {0}; // 图像直方图，256个点
  int i, j, k;
  int n, n1, n2, gmin, gmax;
  float m1, m2, sum, csum, fmax, sb;

  gmin = 200;
  gmax = 60;
  // 生成直方图
  for (i = 0; i < MT9V03X_H - 1; i++) {
    np = &image[i * MT9V03X_W + 1];
    for (j = 0; j < MT9V03X_W - 4; j++) //丢掉右边的四列
    {
      ihist[*np]++;
      if (*np > gmax)
        gmax = *np;
      if (*np < gmin)
        gmin = *np;
      np++;
    }
  }
  sum = csum = 0.0;
  n = 0; //像素点的个数
  for (k = 0; k <= 255; k++) {
    sum += (float)k * (float)ihist[k]; // x*f(x) 质量矩
    n += ihist[k]; /* f(x) 质量 */
  }
  if (!n) //如果像素点为0
  {
    return (160);
  }
  fmax = -1.0;
  n1 = 0;
  for (k = 0; k < 255; k++) {
    n1 += ihist[k];
    if (!n1) {
      continue;
    }
    n2 = n - n1;
    if (n2 == 0) {
      break;
    }
    csum += (double)k * ihist[k];
    m1 = csum / n1;
    m2 = (sum - csum) / n2;
    sb = (double)n1 * (double)n2 * (m1 - m2) * (m1 - m2);

    if (sb > fmax) {
      fmax = sb;
      thresholdValue = k;
    }
  }
  return (thresholdValue);
}

void getUseImage(void) {
  uint8 threshold = 0;
  threshold = GetOSTU_v2(Image[0]);

  for (uint8 i = 0; i < MT9V03X_H; i++) {
    for (uint8 j = 0; j < MT9V03X_W; j++) {
      if (Image[i][j] > threshold) {
        // mt9v03x_image[i][j]=1;   //白色
        Image[i][j] = 255;
      } else {
        // mt9v03x_image[i][j]=0;     //黑色
        Image[i][j] = 0;
      }
    }
  }
  // ImageFilter();
}

//三面以上反数围绕清除噪点
void ImageFilter() {
  int i; //行
  int j; //列

  for (i = 1; i < 40; i++) // 128
  {
    for (j = 1; j < 99; j++) // 188
    {
      if ((Image[i][j] == 0) && (Image[i - 1][j] + Image[i + 1][j] +
                                     Image[i][j + 1] + Image[i][j - 1] >
                                 510)) {
        Image[i][j] = 255;
      } else if ((Image[i][j] == 1) && (Image[i - 1][j] + Image[i + 1][j] +
                                            Image[i][j + 1] + Image[i][j - 1] <
                                        510)) {
        Image[i][j] = 0;
      }
    }
  }
}

void setRoadShow() //显示赛道中线
{
  uint8 h = 0;
  for (h = RowMax; h > 0; h--) {
    Image[h][MiddleLine[h]] = 0;
    //        Image[h][LeftEdge[h]+3] = 100;
    Image[h][RightEdge[h] - 5] = 100;
    // Image[h][MiddleLine[h]] = 0;
    // ips114_drawpoint(center[h],59-h,RED);
  }
}

void Camera_Init() {
  ImageFilter();
  getUseImage();
}
void Camera_deal(void) {
  SetInitVal(); //中线搜索函数初始化
  judge_eg();
  WD_judge();
  check_starting_line();
  chalupanduan();
  setRoadShow();
}
