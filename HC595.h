/*************************************************
  Copyright (C), 刘慧鹏
  File name:HC595.h
  Author:刘慧鹏    Version :2.0    Date:2016/12/12
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. void hc595_init(void)                //HC595初始化
    2. void hc595_write_byte(uint8_t byte)  //HC595数据写入
    3. void display(int32_t display_floor)  //共阴数码管显示楼层
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:2016/12/12
       Author:刘慧鹏
       Modification:根据V2.0板子修改595的I/O口，添加OE口
 
    2. ...
*************************************************/

#ifndef __HC595_H
#define __HC595_H

#include "stm32f10x_conf.h"
//74HC595
//GPIOB
#define HC595_SDI   GPIO_Pin_8      // serial data input
#define HC595_OE    GPIO_Pin_9      // out enable
//GPIOE
#define HC595_SCLK  GPIO_Pin_1      // shift register clock input
#define HC595_RCLK  GPIO_Pin_0      // storage register clock input

void hc595_init(void);
void hc595_write_byte(uint8_t byte);
void display(int32_t display_floor);

#endif
