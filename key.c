/************************************************************
  Copyright (C), 刘慧鹏
  FileName: key.c
  Author:刘慧鹏    Version :1.1    Date:2016/11/28
  Description:LED灯               // 模块描述     
  Version:完成所有按键扫描，扫描之后的函数未做
  Function List:   // 主要函数及其功能
    1. void key_init()                              //按键GPIO初始化
    2. void Key_Scan()                              //按键扫描
    3. void keyc_short_press(uint32_t key_value);   //GPIOC组按键短按处理程序
    4. void keyc_long_press(uint32_t key_value);    //GPIOC组按键长按处理程序
    5. void keyb8_short_press(void);                //GPIOB.8按键短按处理程序
    6. void keyb8_long_press(void);                 //GPIOB.8按键长按处理程序
    7. void keyb9_short_press(void);                //GPIOB.9按键短按处理程序
    8. void keyb9_long_press(void);                 //GPIOB.8按键长按处理程序
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

void key_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口(GPIOC)的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 \
                                    |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //上拉输入，按键之后接地
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /*开启按键端口(GPIOB.8)的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //FT,5V容忍，浮空输入
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*开启按键端口(GPIOB.9)的时钟*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //上拉输入，按键之后接地
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Key_Scan()
{
	static enum key_states_e keyc_state=KEY_S1;         //GPIOC组IO状态
	static enum key_states_e keyb8_state=KEY_S1;        //GPIOB.8状态    
	static enum key_states_e keyb9_state=KEY_S1;        //GPIOB.9状态
	static int pressc = 0;                              //GPIOC组IO长按计数
    static int pressb8 = 0;                             //GPIOB.8长按计数
    static int pressb9 = 0;                             //GPIOB.9长按计数
    static uint32_t keyc_value_S1 = 0;                  //记录GPIOC组IO状态
    static bool long_press_flagc = false;               //长按函数执行标志，只执行一次
    static bool long_press_flagb8 = false;
    static bool long_press_flagb9 = false;

    //以下扫描程序使用状态机，备注不好写，具体可参考以下链接
    //http://blog.csdn.net/lanmanck/article/details/8707957
    /*GPIOC 输入处理。按键和接触开关处理，通常状态为1，闭合为0*/
	switch(keyc_state)
	{
		case KEY_S1:
            keyc_value_S1 = GPIO_ReadInputData(GPIOC);
			if(((keyc_value_S1 & (0xffff)) != 0xffff))
			{
                keyc_state = KEY_S2;
			}
			else
				keyc_state = KEY_S1;
			break;

		case KEY_S2:
			if( keyc_value_S1 == GPIO_ReadInputData(GPIOC) )
            {
				keyc_state = KEY_S3;
                keyc_short_press(keyc_value_S1);                      //短按少于500ms处理程序
			}else
				keyc_state = KEY_S1;

			break;

		case KEY_S3:
			if(keyc_value_S1 == GPIO_ReadInputData(GPIOC))
            {
				keyc_state = KEY_S3;
				pressc++;
				if((pressc>50) && (long_press_flagc == false))        //仅执行一次长按操作
                {
                    keyc_long_press(keyc_value_S1);                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flagc = true;
				}
			}
			else
            {
				keyc_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(keyc_value_S1 == GPIO_ReadInputData(GPIOC))
            {
				keyc_state = KEY_S2;
			}
            else
            {
				keyc_state = KEY_S1;
            }
            pressc = 0;
            keyc_value_S1 = 0xffff;
            long_press_flagc = false;
			break;

		default:
			keyc_state = KEY_S1;
			pressc = 0;
			break;
	}
    
    /*GPIOB.8处理。接近开关处理，通常状态为1，接近金属为0*/
    switch(keyb8_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
			{
                keyb8_state = KEY_S2;
			}
			else
				keyb8_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
            {
				keyb8_state = KEY_S3;
                keyb8_short_press();                      //短按少于500ms处理程序
			}else
				keyb8_state = KEY_S1;

			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
            {
				keyb8_state = KEY_S3;
				pressb8++;
				if((pressb8>50) && (long_press_flagb8 == false))        //仅执行一次长按操作
                {
                    keyb8_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flagb8 = true;
				}
			}
			else
            {
				keyb8_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
            {
				keyb8_state = KEY_S2;
			}
            else
            {
				keyb8_state = KEY_S1;
            }
            pressb8 = 0;
            long_press_flagb8 = false;
			break;

		default:
			keyb8_state = KEY_S1;
			pressb8 = 0;
			break;
	}
    
    /*GPIOB.9处理。接触开关处理，通常状态为1，闭合为0*/
    switch(keyb9_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
			{
                keyb9_state = KEY_S2;
			}
			else
				keyb9_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
            {
				keyb9_state = KEY_S3;
                keyb9_short_press();                      //短按少于500ms处理程序
			}else
				keyb9_state = KEY_S1;

			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
            {
				keyb9_state = KEY_S3;
				pressb9++;
				if((pressb9>50) && (long_press_flagb9 == false))        //仅执行一次长按操作
                {
                    keyb9_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flagb9 = true;
				}
			}
			else
            {
				keyb9_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
            {
				keyb9_state = KEY_S2;
			}
            else
            {
				keyb9_state = KEY_S1;
            }
            pressb9 = 0;
            long_press_flagb9 = false;
			break;

		default:
			keyb9_state = KEY_S1;
			pressb9 = 0;
			break;
	}
}

void keyc_short_press(uint32_t key_value)
{
    led_1SWL(ON);
}

void keyc_long_press(uint32_t key_value)
{
    led_1SWL(OFF);
}

void keyb8_short_press()
{
    led_2SWL(ON);
}

void keyb8_long_press()
{
    led_2SWL(OFF);
}

void keyb9_short_press()
{
    led_3SWL(ON);
}

void keyb9_long_press()
{
    led_3SWL(OFF);
}
