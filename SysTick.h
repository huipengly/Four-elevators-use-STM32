/*************************************************
  Copyright (C), 刘慧鹏
  File name:SysTick.h
  Author:刘慧鹏    Version :1.0    Date:2016/12/04
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. void SysTick_Init(void)          //滴答定时器初始化
    2. void Delay_us(__IO u32 nTime)    //延时10us*nTime
    3. Delay_ms(x)                      //延时1ms*x
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:
       Author:
       Modification:
 
    2. ...
*************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
#define Delay_ms(x) Delay_us(100*x)	 //单位ms

#endif /* __SYSTICK_H */
