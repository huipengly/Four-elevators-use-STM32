/************************************************************
  Copyright (C), 刘慧鹏
  FileName: led.c
  Author:刘慧鹏    Version :2.0    Date:2016/12/12
  Description:LED灯               // 模块描述     
  Version:1.0                     // 版本信息
  Function List:   // 主要函数及其功能
    1. void led_init()      //初始化led灯GPIO
    2. void led_XXX()       //控制LED通断。ON/OFF
  History:         // 历史修改记录
      <author>  <time>      <version>   <desc>
      刘慧鹏    2016/12/12  2.0          V2.0pcb版I/O修改
***********************************************************/

#include "include.h"

extern int32_t up_down_flag;           //0代表下降，1代表上升，2代表停止。

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
    
    // /*开复用时钟，将复用口作为普通IO*/
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
    /*开启GPIO外设时钟*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
    
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	
    
    /*初始化GPIOA的引脚*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    
    /*初始化GPIOB的引脚*/
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
    
    /*初始化GPIOD的引脚*/
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*选择引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
    
    /*初始化GPIOD的引脚*/
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /*关闭所有IO输出*/
    led_1SWL(OFF);
    led_2SWL(OFF);
    led_4DL(OFF);
    led_3SWL(OFF);
    led_4SWL(OFF);
    led_LIGHT(OFF);
    led_RUNL(OFF);
    led_DL(OFF);
    led_UL(OFF);
    led_CLOSEL(OFF);
    led_3UL(OFF);
    led_3DL(OFF);
    led_2UL(OFF);
    led_2DL(OFF);
    led_1UL(OFF);
    led_OPENL(OFF);
    led_STOPL(OFF);
    led_STATE(OFF);
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
        GPIO_SetBits(GPIOA,GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_5);
    }
}

void led_2SWL(int state)
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

void led_4DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_13);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_13);
    }
}

void led_3SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_14);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_14);
    }
}

void led_4SWL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_15);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_15);
    }
}

void led_LIGHT(int state)
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

void led_RUNL(int state)
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

void led_DL(int state)
{
    if(state)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    }
    else
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_7);
    }
}

void led_UL(int state)
{
    if(state)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    }
    else
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_6);
    }
}

void led_up_down_state(void)
{
    if(UP == up_down_flag)
    {
        led_UL(ON);
        led_DL(OFF);
    }
    else if(DOWN == up_down_flag)
    {
        led_UL(OFF);
        led_DL(ON);
    }
    else if(STAY == up_down_flag)
    {
        led_UL(OFF);
        led_DL(OFF);
    }
}

void led_CLOSEL(int state)
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

void led_3UL(int state)
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

void led_3DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_8);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_8);
    }
}

void led_2UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_9);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_9);
    }
}

void led_2DL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_10);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_10);
    }
}

void led_1UL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_11);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_11);
    }
}

void led_OPENL(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOD,GPIO_Pin_12);
    }
}

void led_STOPL(int state)
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

void led_STATE(int state)
{
    if(state)
    {
        GPIO_SetBits(GPIOE,GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOE,GPIO_Pin_2);
    }
}
