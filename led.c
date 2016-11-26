/************************************************************
  Copyright (C), 刘慧鹏
  FileName: led.c
  Author:刘慧鹏    Version :1.0    Date:2016/11/26
  Description:主函数               // 模块描述     
  Version:初始化工程               // 版本信息
  Function List:   // 主要函数及其功能
    1. -------
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
