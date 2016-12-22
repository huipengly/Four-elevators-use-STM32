/************************************************************
  Copyright (C), 刘慧鹏
  FileName: motor.c
  Author:刘慧鹏    Version :1.0    Date:2016/12/22
  Description:V2.0主控板电机控制               // 模块描述
  Version:               // 版本信息
  Function List:   // 主要函数及其功能
    1. -------
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/
#include "include.h"

void motor_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 设置TIM2.3CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE); 

    /* GPIOA clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /*GPIOA Configuration: TIM2 channel 1~4 & TIM3 channel 1 and 2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
}

void pwm_config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    /* Time base configuration */		 
    TIM_TimeBaseStructure.TIM_Period = 1000-1;        //1KHz频率
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //设置预分频：72分频，即为1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//配置定时器2
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//配置定时器2

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	            //配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
    TIM_OCInitStructure.TIM_Pulse = 0;	                        //设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //当定时器计数值小于value时为高电平

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //使能通道1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);	 //使能通道2
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道3
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);	 //使能通道4
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_Cmd(TIM2, ENABLE); //使能定时器2
    
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //使能通道2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);	 //使能通道4
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_Cmd(TIM3, ENABLE); //使能定时器3
}

void motor_init(void)
{
    motor_gpio_init();
    pwm_config();
}

void lift_wait(void)
{
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    TIM2->CCR4 = 0;
    Delay_ms(10);
}

void lift_up(void)
{
    lift_wait();
    TIM2->CCR1 = 900;
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    TIM2->CCR4 = 900;
}

void lift_down(void)
{
    lift_wait();
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 900;
    TIM2->CCR3 = 900;
    TIM2->CCR4 = 0;
}

void lift_stop(void)
{
    lift_wait();
    TIM2->CCR1 = 1000;
    TIM2->CCR2 = 1000;
    TIM2->CCR3 = 0;
    TIM2->CCR4 = 0;
}

void door_wait(void)
{
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;
    Delay_ms(10);
}

void door_open(void)
{
    door_wait();
    TIM3->CCR1 = 900;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 900;
}

void door_close(void)
{
    door_wait();
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 900;
    TIM3->CCR3 = 900;
    TIM3->CCR4 = 0;
}

void door_stop(void)
{
    door_wait();
    TIM3->CCR1 = 1000;
    TIM3->CCR2 = 1000;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;
}
