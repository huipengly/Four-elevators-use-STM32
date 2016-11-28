/************************************************************
  Copyright (C), 刘慧鹏
  FileName: key.c
  Author:刘慧鹏    Version :1.0    Date:2016/11/27
  Description:LED灯               // 模块描述     
  Version:1.0                     // 版本信息
  Function List:   // 主要函数及其功能
    1. 
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

void key_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口（PC）的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 \
                                    |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Key_Scan(GPIO_TypeDef* GPIOx)
{
	static enum key_states_e key_state=KEY_S1;
	static int press=0;
    static uint32_t key_value_S1 = 0;
    static bool long_press_flag = false;               //长按只执行一次
//    static uint32_t key_value_S2 = 0xffff;

	switch(key_state)
	{
		case KEY_S1:
            key_value_S1 = GPIO_ReadInputData(GPIOx);
			if(((key_value_S1 & (0xffff)) != 0xffff))
			{
                key_state = KEY_S2;
			}
			else
				key_state = KEY_S1;
			break;

		case KEY_S2:
//            key_value_S2 = GPIO_ReadInputData(GPIOx);
			if( key_value_S1 == GPIO_ReadInputData(GPIOx) )
            {
				key_state = KEY_S3;
                key_short_press(key_value_S1);                      //短按少于500ms处理程序
			}else
				key_state = KEY_S1;

			break;

		case KEY_S3:
			if(key_value_S1 == GPIO_ReadInputData(GPIOx))
            {
				key_state = KEY_S3;
				press++;
				if((press>50) && (long_press_flag == false))        //仅执行一次长按操作
                {
                    key_long_press(key_value_S1);                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag = true;
				}
			}
			else
            {
				key_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(key_value_S1 == GPIO_ReadInputData(GPIOx))
            {
				key_state = KEY_S2;
			}
            else
            {
				key_state = KEY_S1;
            }
            press = 0;
            key_value_S1 = 0xffff;
            long_press_flag = false;
			break;

		default:
			key_state = KEY_S1;
			press = 0;
			break;
	}
}

void key_short_press(uint32_t key_value)
{
    led_1SWL(ON);
}

void key_long_press(uint32_t key_value)
{
    led_1SWL(OFF);
}
