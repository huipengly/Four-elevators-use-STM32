/*************************************************
  Copyright (C), 刘慧鹏
  File name:motor.h
  Author:刘慧鹏    Version :1.0    Date:2016/12/22
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. 
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. 
*************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

void motor_gpio_init(void);
void pwm_config(void);
void motor_init(void);
void lift_up(void);
void lift_down(void);
void lift_stop(void);
void door_open(void);
void door_close(void);
void door_stop(void);

#endif
