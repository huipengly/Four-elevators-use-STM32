/*************************************************
  Copyright (C), 刘慧鹏
  File name:key.h
  Author:刘慧鹏    Version :2.0    Date:2016/12/12
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  Others:         // 其它内容的说明
  Function List:  // 主要函数列表，每条记录应包括函数名及功能简要说明
    1. void key_init()                              //按键GPIO初始化
    2. void Key_Scan()                              //按键扫描
    3. void keyc_short_press(uint32_t key_value);   //GPIOC组按键短按处理程序
    4. void keyc_long_press(uint32_t key_value);    //GPIOC组按键长按处理程序
    5. void keyb8_short_press(void);                //GPIOB.8按键短按处理程序
    6. void keyb8_long_press(void);                 //GPIOB.8按键长按处理程序
    7. void keyb9_short_press(void);                //GPIOB.9按键短按处理程序
    8. void keyb9_long_press(void);                 //GPIOB.8按键长按处理程序
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述 
    1. Date:2016/12/12
       Author:刘慧鹏
       Modification:根据V2.0板子修改I/O口
 
    2. ...
*************************************************/

#ifndef __KEY_H
#define __KEY_H

#include <stm32f10x_conf.h>
#include <stdbool.h>                        //bool量

#define key_1SW_gpio        GPIOC,GPIO_Pin_0
#define key_2SW_gpio        GPIOC,GPIO_Pin_15
#define key_3SW_gpio        GPIOC,GPIO_Pin_14
#define key_4SW_gpio        GPIOC,GPIO_Pin_13
#define key_OPEN_gpio       GPIOC,GPIO_Pin_4
#define key_CLOSE_gpio      GPIOC,GPIO_Pin_5
#define key_RUN_gpio        GPIOC,GPIO_Pin_6
#define key_STOP_gpio       GPIOC,GPIO_Pin_7
#define key_4DS_gpio        GPIOC,GPIO_Pin_8
#define key_3DS_gpio        GPIOC,GPIO_Pin_9
#define key_3US_gpio        GPIOC,GPIO_Pin_10
#define key_2DS_gpio        GPIOC,GPIO_Pin_11
#define key_2US_gpio        GPIOC,GPIO_Pin_12
#define key_1US_gpio        GPIOC,GPIO_Pin_3
#define key_DROUND_gpio     GPIOC,GPIO_Pin_2
#define key_DOPEN_gpio      GPIOC,GPIO_Pin_1
#define key_DFLOOR_gpio     GPIOE,GPIO_Pin_7
#define key_DCLOSE_gpio     GPIOA,GPIO_Pin_4

enum key_states_e{
	KEY_S1,
	KEY_S2,
	KEY_S3,
	KEY_S4
};                                          //四种键盘状态，未按下，按下，保持按下，松开

void key_init(void);                        //按键GPIO初始化
void key_scan(void);                        //按键扫描
void keyc_short_press(uint32_t key_value);  //GPIOC组按键短按处理程序
void keyc_long_press(uint32_t key_value);   //GPIOC组按键长按处理程序
void key_dfloor_short_press(void);               //GPIOB.8按键短按处理程序
void key_dfloor_long_press(void);                //GPIOB.8按键长按处理程序
void key_dclose_short_press(void);               //GPIOB.9按键短按处理程序
void key_dclose_long_press(void);                //GPIOB.8按键长按处理程序
void key_1SW_scan(void);

#endif
