/*************************************************
  Copyright (C), 刘慧鹏
  File name:led.h
  Author:刘慧鹏    Version :2.0    Date:2016/12/12
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. ....
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:2016/12/12
       Author:刘慧鹏
       Modification:根据V2.0板子修改I/O
                    添加一个状态显示LED
 
    2. ...
*************************************************/

#ifndef __LED_H
#define __LED_H

#include "stm32f10x_conf.h"

//定义ON/OFF控制LED开关
#define ON  1
#define OFF 0

void led_init(void);
void led_1SWL(int state);
void led_2SWL(int state);
void led_4DL(int state);
void led_3SWL(int state);
void led_4SWL(int state);
void led_LIGHT(int state);
void led_RUNL(int state);
void led_DL(int state);
void led_UL(int state);
void led_CLOSEL(int state);
void led_3UL(int state);
void led_3DL(int state);
void led_2UL(int state);
void led_2DL(int state);
void led_1UL(int state);
void led_OPENL(int state);
void led_STOPL(int state);
void led_STATE(int state);

#endif
