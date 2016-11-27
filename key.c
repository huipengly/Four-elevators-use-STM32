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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	static enum key_states_e key_state=KEY_S1;
	static int press=0;

	switch(key_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
				{key_state = KEY_S2;
			}
			else
				key_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){
				key_state = KEY_S3;                 //短按处理程序
                led_1SWL(ON);
			}else
				key_state = KEY_S1;

			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){
				key_state = KEY_S3;
				press++;
				if(press>50){
                    led_1SWL(OFF);                  //长按200ms处理程序
				}
			}
			else
				key_state = KEY_S4;

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON){
				key_state = KEY_S1;
				press = 0;
			}
			break;

		default:
			key_state = KEY_S1;
			press = 0;
			break;
	}
}
