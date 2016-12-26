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
extern int32_t lift_init_ok;            //电梯初始化标志位，0表示还未降到最底层，1表示降到最底层，初始化完成
extern int32_t lift_floor;
extern int32_t up_down_flag;            //0代表下降，1代表上升。
extern int32_t lift_state;              //电梯运行状态,RUN/STOP

#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
#define LED1_TOGGLE		digitalToggle(GPIOD,GPIO_Pin_11)

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
    motor_init();                   //电机初始化
    hc595_init();                   //hc595初始化
//    door_motor_close();
    while(1)
    {
        run();
    }
}

void run()
{
    static uint32_t state = 1;
//    static uint32_t stop_time = 0;
    if(time_10ms >= 10)  //10ms
    {
        key_scan();
        time_10ms = 0;
    }
    
    if(time_100ms >= 100)
    {
        state = !state;
        led_STATE(state);
        lift();
        time_100ms = 0;
    }
    
//    if(time_100ms >= 100)  //100ms
//    {
//        state = !state;
//        led_STATE(state);
////        led_UL(state);
////        led_DL(state);
////        hc595_write_byte(aa);
////        display(aa);
////        aa++;
////        if(aa == 5)
////            aa = 1;
//        if( RUN == lift_state )
//        {
//            if( 1 == lift_init_ok )
//            {
//                lift_motor_stop();
//                up_down_flag = UP;
//                lift_floor = 0;
//                lift_init_ok = 2;
//                lift_motor_up();
//            }
//            if((2 == lift_init_ok) && (1 == lift_floor))
//            {
//                lift_motor_stop();
//                lift_init_ok = 3;
//            }
//            if(3 == lift_init_ok)
//            {
//                lift();
//                display(lift_floor);
//            }
//            if(4 == lift_init_ok)
//            {
//                //当平层时进入状态4
//                lift_motor_stop();//停止电梯,开门，开定时器，lift_init_ok变为5
//                stop_time = 0;
//                lift_init_ok = 5;
//                door_motor_open();
//            }
//            if(5 == lift_init_ok)
//            {
////                if(1 == OPENED)
////                {
////                    door_motor_stop();
////                    lift_init_ok = 6;
////                }
//            }
//            if(6 == lift_init_ok)
//            {
////                stop_time++;
////                if((20 <= stop_time) && (1 == CLOSED))
////                {
////                    lift_init_ok = 3;
////                }
////                else if(20 <= stop_time)//2s之后关门，关门完成之后返回状态3
////                {
////                    door_motor_close();
////                }
//            }
//        }
//        else 
//        {
//            display(0xff);
//        }
//        time_100ms = 0;
//    }

}
