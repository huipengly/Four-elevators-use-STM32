/*************************************************
  Copyright (C), 刘慧鹏
  File name:key.h
  Author:刘慧鹏    Version :1.0    Date:2016/11/27
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. ....
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:
       Author:
       Modification:
 
    2. ...
*************************************************/

#ifndef __KEY_H
#define __KEY_H

#include <stm32f10x_conf.h>

#define KEY_ON	0
#define KEY_OFF	1

enum key_states_e{
	KEY_S1,
	KEY_S2,
	KEY_S3,
	KEY_S4
};

void key_init(void);
void Key_Scan(GPIO_TypeDef* GPIOx);
void key_short_press(uint32_t key_value);
void key_long_press(uint32_t key_value);

#endif
