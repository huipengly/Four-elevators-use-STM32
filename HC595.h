/*************************************************
  Copyright (C), 刘慧鹏
  File name:HC595.h
  Author:刘慧鹏    Version :1.0    Date:2016/12/04
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. void hc595_init(void)            //滴答定时器初始化
    2. void hc595_write_byte(uint8_t byte)
                                        //延时10us*nTime
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:
       Author:
       Modification:
 
    2. ...
*************************************************/

#ifndef __HC595_H
#define __HC595_H

#include "stm32f10x_conf.h"
//74HC595
//#define HC595_PORT  GPIOA
#define HC595_DATA  GPIO_Pin_6      // serial data input, pin14
#define HC595_SCLK  GPIO_Pin_7      // shift register clock input, pin11
#define HC595_RCLK  GPIO_Pin_0      // storage register clock input, pin12

void hc595_init(void);
void hc595_write_byte(uint8_t byte);

#endif
