/*************************************************
  Copyright (C), 刘慧鹏
  File name:timer.h
  Author:刘慧鹏    Version :1.0    Date:2016/11/26
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. void TIM4_NVIC_Configuration(void);  //配置中断优先级
    2. void TIM4_Configuration(void);       //配置定时器4，1ms进一次中断
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:
       Author:
       Modification:
 
    2. ...
*************************************************/

#ifndef __TIMER_H
#define __TIMER_H

void TIM4_NVIC_Configuration(void);
void TIM4_Configuration(void);

#endif
