/************************************************************
  Copyright (C), 刘慧鹏
  FileName: led.c
  Author:刘慧鹏    Version :1.0    Date:2016/11/26
  Description:LED灯               // 模块描述     
  Version:1.0                     // 版本信息
  Function List:   // 主要函数及其功能
    1. void led_init()      //初始化led灯GPIO
    2. void led_XXX()       //控制LED通断。ON/OFF
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

/*************************************************
  Function:     void led_init()
  Description:  初始化led灯GPIO
  Calls:          // 被本函数调用的函数清单
  Called By:      // 调用本函数的函数清单
  Others:         // 其它说明
*************************************************/
void led_init()
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*开复用时钟，将复用口作为普通IO*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
    /*开启GPIO外设时钟*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);
    
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	
    
    /*初始化GPIOA的引脚*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    
    /*初始化GPIOB的引脚*/
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
    
    /*初始化GPIOD的引脚*/
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*关闭所有IO输出*/
    GPIO_ResetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
    GPIO_ResetBits(GPIOB, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

/*************************************************
  Function:     void led_XXX()
  Description:  控制LED通断。ON/OFF
  Calls:          // 被本函数调用的函数清单
  Called By:      // 调用本函数的函数清单
  Others:         // 其它说明
*************************************************/

void led_1SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_4);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    }
}

void led_2SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_5);
    }
}

void led_4DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_8);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_8);
    }
}

void led_3SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_11);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_11);
    }
}

void led_4SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_12);
    }
}

void led_LIGHT(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_15);
    }
}

void led_RUNL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);
    }
}

void led_DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_3);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    }
}

void led_UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_4);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    }
}

void led_CLOSEL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_5);
    }
}

void led_3UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_10);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    }
}

void led_3DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_11);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_11);
    }
}

void led_2UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_12);
    }
}

void led_2DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_13);
    }
}

void led_1UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_14);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_14);
    }
}

void led_OPENL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_15);
    }
}

void led_STOPL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
    }
}
