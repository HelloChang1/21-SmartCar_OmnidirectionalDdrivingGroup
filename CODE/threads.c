#include "threads.h"

//线程控制块
rt_err_t result;
rt_sem_t camera_sem;
rt_thread_t camera_thread;

rt_thread_t speed_thread;
rt_thread_t gyro_thread;
rt_thread_t elec_thread;

void camera_thread_entry(void *parameter) {
  //线程控制块指针
  rt_thread_t tid = rt_thread_find("camera_thread");

  while (1) {
    //等待摄像头采集完毕
    rt_sem_take(camera_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(5);
    //开始处理摄像头图像
    Camera_Init(); //摄像头采集的图像降噪以及进行二值化
    Camera_deal(); //摄像头对赛道元素进行识别
  }
  rt_kprintf("\n camera_thread exit.\n");
}

void speed_thread_entry(void *parameter) {
  //线程控制块指针
  rt_thread_t tid = rt_thread_find("speed_thread");

  rt_thread_mdelay(5); //等待FINSH初始化完毕

  while (1) {
    SpeedGet();
  }
  rt_kprintf("\n speed_thread exit.\n");
}

void gyro_thread_entry(void *parameter) {
  //线程控制块指针
  rt_thread_t tid = rt_thread_find("gyro_thread");

  rt_thread_mdelay(5); //等待FINSH初始化完毕

  while (1) {
    get_icm20602_gyro_spi();
  }
  rt_kprintf("\n gyro_thread exit.\n");
}

void elec_thread_entry(void *parameter) {
  //线程控制块指针
  rt_thread_t tid = rt_thread_find("elec_thread");

  rt_thread_mdelay(5); //等待FINSH初始化完毕

  while (1) {
    elec_get();
  }
  rt_kprintf("\n elec_thread exit.\n");
}



int SensorData_Threads_Init(void) {
  //创建动态线程
  camera_sem = rt_sem_create("camera", 0, RT_IPC_FLAG_FIFO);
  camera_thread = rt_thread_create(
      "camera_thread",    //线程名称
      speed_thread_entry, //线程入口函数
      RT_NULL,            //线程参数
      512, // 512个字节的栈空间
      5, //线程优先级为5，数值越小，优先级越高，0为最高优先级。
      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
      5); //时间片为5

  rt_kprintf("create dynamic thread.\n");
  if (camera_thread != RT_NULL) //线程创建成功
  {
    rt_kprintf("camera_thread dynamic thread create OK.\n");
    rt_thread_startup(camera_thread); //运行该线程
  } else                              //线程创建失败
  {
    rt_kprintf("camera_thread dynamic thread create ERROR.\n");
    return 1;
  }

  speed_thread = rt_thread_create(
      "speed_thread",     //线程名称
      speed_thread_entry, //线程入口函数
      RT_NULL,            //线程参数
      512, // 512个字节的栈空间
      6, //线程优先级为6，数值越小，优先级越高，0为最高优先级。
      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
      5); //时间片为5

  rt_kprintf("create dynamic thread.\n");
  if (speed_thread != RT_NULL) //线程创建成功
  {
    rt_kprintf("speed_thread dynamic thread create OK.\n");
    rt_thread_startup(speed_thread); //运行该线程
  } else                             //线程创建失败
  {
    rt_kprintf("speed_thread dynamic thread create ERROR.\n");
    return 1;
  }

  gyro_thread = rt_thread_create(
      "gyro_thread",     //线程名称
      gyro_thread_entry, //线程入口函数
      RT_NULL,           //线程参数
      512, // 512个字节的栈空间
      7, //线程优先级为7，数值越小，优先级越高，0为最高优先级。
      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
      5); //时间片为5

  rt_kprintf("create dynamic thread.\n");
  if (gyro_thread != RT_NULL) //线程创建成功
  {
    rt_kprintf("gyro_thread dynamic thread create OK.\n");
    rt_thread_startup(gyro_thread); //运行该线程
  } else                            //线程创建失败
  {
    rt_kprintf("gyro_thread dynamic thread create ERROR.\n");
    return 1;
  }

  elec_thread = rt_thread_create(
      "elec_thread",     //线程名称
      elec_thread_entry, //线程入口函数
      RT_NULL,           //线程参数
      512, // 512个字节的栈空间
      8, //线程优先级为8，数值越小，优先级越高，0为最高优先级。
      //可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
      5); //时间片为5

  rt_kprintf("create dynamic thread.\n");
  if (elec_thread != RT_NULL) //线程创建成功
  {
    rt_kprintf("elec_thread dynamic thread create OK.\n");
    rt_thread_startup(elec_thread); //运行该线程
  } else                            //线程创建失败
  {
    rt_kprintf("elec_thread dynamic thread create ERROR.\n");
    return 1;
  }

  return 0;
}
