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
extern volatile u32 time_10ms;
extern volatile u32 time_100ms;

#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
#define LED1_TOGGLE		digitalToggle(GPIOA,GPIO_Pin_8)

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
    key_init();                     //按键初始化
    usart1_init();                  //串口1初始化
    SysTick_Init();                 //滴答定时器初始化
    hc595_init();                   //hc595初始化
//    hc595_write_byte(0xaa);
    display(4);
    while(1)
    {
//        hc595_write_byte(0x00);
//        Delay_ms(50);
//        hc595_write_byte(0x08);
//        Delay_ms(50);
        run();
    }
}

void run()
{
//    static uint32_t aa = 0xff;
    if(time_10ms == 10)  //10ms
    {
        Key_Scan();
        time_10ms = 0;
    }
    
    if(time_100ms == 100)  //100ms
    {
//        aa = ~aa;
//        hc595_write_byte(aa);
        LED1_TOGGLE;
        time_100ms = 0;
    }

//下面代码有bug，不知为什么 
//    if((time%10) == 0)  //10ms
//    {
//        Key_Scan();
//    }
//    if(time == 100)
//    {
//        time = 1;
//    }

}
