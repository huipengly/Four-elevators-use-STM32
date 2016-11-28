/************************************************************
  Copyright (C), 刘慧鹏
  FileName: main.c
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
extern volatile u32 time;

#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
#define LED1_TOGGLE		digitalToggle(GPIOA,GPIO_Pin_4)

void run(void);

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

int main()
{
    led_init();                     //led初始化
    TIM4_Configuration();           //配置定时器4
	TIM4_NVIC_Configuration();      //设置定时器中断优先级
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);   
                                    //TIM4重新开时钟，开始计时
    key_init();
    while(1)
    {
        run();
    }
}

void run()
{
    if(time == 10)  //10ms
    {
        Key_Scan(GPIOC);
        time = 0;
    }
}