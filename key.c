/************************************************************
  Copyright (C), 刘慧鹏
  FileName: key.c
  Author:刘慧鹏    Version :2.1    Date:2016/12/25
  Description:LED灯               // 模块描述     
  Version:完成所有按键扫描，扫描之后的函数未做
  Function List:   // 主要函数及其功能
    1. void key_init()                              //按键GPIO初始化
    2. void Key_Scan()                              //按键扫描
    3. void keyc_short_press(uint32_t key_value);   //GPIOC组按键短按处理程序
    4. void keyc_long_press(uint32_t key_value);    //GPIOC组按键长按处理程序
    5. void key_dfloor_short_press(void);                //GPIOB.8按键短按处理程序
    6. void key_dfloor_long_press(void);                 //GPIOB.8按键长按处理程序
    7. void key_dclose_short_press(void);                //GPIOB.9按键短按处理程序
    8. void key_dclose_long_press(void);                 //GPIOB.8按键长按处理程序
  History:         // 历史修改记录
      <author>  <time>      <version>   <desc>
      刘慧鹏    2016/12/12  2.0          V2.0pcb版I/O修改
      刘慧鹏    2016/12/25  2.1          修改按键为单独扫描，不冲突
***********************************************************/

#include "include.h"

extern int32_t lift_penal[4];       //1SW/2SW/3SW/4SW
extern int32_t floor_penal[2][4];   //1DS/2DS/3DS/4DS
                                    //1US/2US/3US/4US
//extern int32_t state_penal[4];      //OPEN/CLOSE/RUN/STOP
//extern int32_t RUN;
//extern int32_t STOP;
extern int32_t door_open;               //开门信号    
extern int32_t door_opened;             //门开启信号    
extern int32_t door_close;              //关门信号
extern int32_t door_closed;             //门关闭信号
extern int32_t door_ground_flag;        //轿厢是否到最底层
extern int32_t lift_floor;
extern int32_t up_down_flag;            //0代表下降，1代表上升。
extern int32_t lift_init_ok;            //电梯初始化标志位，0表示还未降到最底层，1表示降到最底层，初始化完成
extern int32_t lift_state;              //电梯运行状态,RUN/STOP

void key_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口(GPIOC)的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 \
                                    |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //上拉输入，按键之后接地
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /*开启按键端口(GPIOE.7)的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //FT,5V容忍，浮空输入
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /*开启按键端口(GPIOA.4)的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //上拉输入，按键之后接地
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void key_1SW_short_press()
{
    led_1SWL(ON);
    lift_penal[0] = 1;
}

void key_1SW_long_press()
{
    led_1SWL(OFF);
    lift_penal[0] = 0;
}

void key_1SW_scan(void)
{
    static enum key_states_e key_1SW_state = KEY_S1;        //按键I/O状态
    static int32_t press_1SW = 0;                           //按键长按计数
    static int long_press_flag_1SW = false;                     //长按函数执行标志，只执行一次
    
    switch(key_1SW_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_1SW_gpio) == 0)
			{
                key_1SW_state = KEY_S2;
			}
			else
				key_1SW_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_1SW_gpio) == 0)
            {
				key_1SW_state = KEY_S3;
                key_1SW_short_press();                      //短按少于500ms处理程序
			}else
				key_1SW_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_1SW_gpio) == 0)
            {
				key_1SW_state = KEY_S3;
				press_1SW++;
				if((press_1SW>50) && (long_press_flag_1SW == false))        //仅执行一次长按操作
                {
                    key_1SW_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_1SW = true;
				}
			}
			else
            {
				key_1SW_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_1SW_gpio) == 0)
            {
				key_1SW_state = KEY_S2;
			}
            else
            {
				key_1SW_state = KEY_S1;
            }
            press_1SW = 0;
            long_press_flag_1SW = false;
			break;

		default:
			key_1SW_state = KEY_S1;
			press_1SW = 0;
			break;
	}
}



void key_2SW_short_press()
{
    led_2SWL(ON);
    lift_penal[1] = 1;
}

void key_2SW_long_press()
{
    led_2SWL(OFF);
    lift_penal[1] = 0;
}

void key_2SW_scan(void)
{
    static enum key_states_e key_2SW_state = KEY_S1;        //按键I/O状态
    static int32_t press_2SW = 0;                           //按键长按计数
    static int long_press_flag_2SW = false;                     //长按函数执行标志，只执行一次
    
    switch(key_2SW_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_2SW_gpio) == 0)
			{
                key_2SW_state = KEY_S2;
			}
			else
				key_2SW_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_2SW_gpio) == 0)
            {
				key_2SW_state = KEY_S3;
                key_2SW_short_press();                      //短按少于500ms处理程序
			}else
				key_2SW_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_2SW_gpio) == 0)
            {
				key_2SW_state = KEY_S3;
				press_2SW++;
				if((press_2SW>50) && (long_press_flag_2SW == false))        //仅执行一次长按操作
                {
                    key_2SW_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_2SW = true;
				}
			}
			else
            {
				key_2SW_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_2SW_gpio) == 0)
            {
				key_2SW_state = KEY_S2;
			}
            else
            {
				key_2SW_state = KEY_S1;
            }
            press_2SW = 0;
            long_press_flag_2SW = false;
			break;

		default:
			key_2SW_state = KEY_S1;
			press_2SW = 0;
			break;
	}
}

void key_3SW_short_press()
{
    led_3SWL(ON);
    lift_penal[2] = 1;
}

void key_3SW_long_press()
{
    led_3SWL(OFF);
    lift_penal[2] = 0;
}

void key_3SW_scan(void)
{
    static enum key_states_e key_3SW_state = KEY_S1;        //按键I/O状态
    static int32_t press_3SW = 0;                           //按键长按计数
    static int long_press_flag_3SW = false;                     //长按函数执行标志，只执行一次
    
    switch(key_3SW_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_3SW_gpio) == 0)
			{
                key_3SW_state = KEY_S2;
			}
			else
				key_3SW_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_3SW_gpio) == 0)
            {
				key_3SW_state = KEY_S3;
                key_3SW_short_press();                      //短按少于500ms处理程序
			}else
				key_3SW_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_3SW_gpio) == 0)
            {
				key_3SW_state = KEY_S3;
				press_3SW++;
				if((press_3SW>50) && (long_press_flag_3SW == false))        //仅执行一次长按操作
                {
                    key_3SW_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_3SW = true;
				}
			}
			else
            {
				key_3SW_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_3SW_gpio) == 0)
            {
				key_3SW_state = KEY_S2;
			}
            else
            {
				key_3SW_state = KEY_S1;
            }
            press_3SW = 0;
            long_press_flag_3SW = false;
			break;

		default:
			key_3SW_state = KEY_S1;
			press_3SW = 0;
			break;
	}
}

void key_4SW_short_press()
{
    led_4SWL(ON);
    lift_penal[3] = 1;
}

void key_4SW_long_press()
{
    led_4SWL(OFF);
    lift_penal[3] = 0;
}

void key_4SW_scan(void)
{
    static enum key_states_e key_4SW_state = KEY_S1;        //按键I/O状态
    static int32_t press_4SW = 0;                           //按键长按计数
    static int long_press_flag_4SW = false;                     //长按函数执行标志，只执行一次
    
    switch(key_4SW_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_4SW_gpio) == 0)
			{
                key_4SW_state = KEY_S2;
			}
			else
				key_4SW_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_4SW_gpio) == 0)
            {
				key_4SW_state = KEY_S3;
                key_4SW_short_press();                      //短按少于500ms处理程序
			}else
				key_4SW_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_4SW_gpio) == 0)
            {
				key_4SW_state = KEY_S3;
				press_4SW++;
				if((press_4SW>50) && (long_press_flag_4SW == false))        //仅执行一次长按操作
                {
                    key_4SW_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_4SW = true;
				}
			}
			else
            {
				key_4SW_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_4SW_gpio) == 0)
            {
				key_4SW_state = KEY_S2;
			}
            else
            {
				key_4SW_state = KEY_S1;
            }
            press_4SW = 0;
            long_press_flag_4SW = false;
			break;

		default:
			key_4SW_state = KEY_S1;
			press_4SW = 0;
			break;
	}
}

void key_OPEN_short_press()
{
    led_OPENL(ON);
    door_open = 1;
}

void key_OPEN_long_press()
{
    led_OPENL(OFF);
    door_open = 0;
}

void key_OPEN_scan(void)
{
    static enum key_states_e key_OPEN_state = KEY_S1;        //按键I/O状态
    static int32_t press_OPEN = 0;                           //按键长按计数
    static int long_press_flag_OPEN = false;                     //长按函数执行标志，只执行一次
    
    switch(key_OPEN_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_OPEN_gpio) == 0)
			{
                key_OPEN_state = KEY_S2;
			}
			else
				key_OPEN_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_OPEN_gpio) == 0)
            {
				key_OPEN_state = KEY_S3;
                key_OPEN_short_press();                      //短按少于500ms处理程序
			}else
				key_OPEN_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_OPEN_gpio) == 0)
            {
				key_OPEN_state = KEY_S3;
				press_OPEN++;
				if((press_OPEN>50) && (long_press_flag_OPEN == false))        //仅执行一次长按操作
                {
                    key_OPEN_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_OPEN = true;
				}
			}
			else
            {
				key_OPEN_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_OPEN_gpio) == 0)
            {
				key_OPEN_state = KEY_S2;
			}
            else
            {
				key_OPEN_state = KEY_S1;
            }
            press_OPEN = 0;
            long_press_flag_OPEN = false;
			break;

		default:
			key_OPEN_state = KEY_S1;
			press_OPEN = 0;
			break;
	}
}

void key_CLOSE_short_press()
{
    led_CLOSEL(ON);
    door_close = 1;
}

void key_CLOSE_long_press()
{
    led_CLOSEL(OFF);
    door_close = 0;
}

void key_CLOSE_scan(void)
{
    static enum key_states_e key_CLOSE_state = KEY_S1;        //按键I/O状态
    static int32_t press_CLOSE = 0;                           //按键长按计数
    static int long_press_flag_CLOSE = false;                     //长按函数执行标志，只执行一次
    
    switch(key_CLOSE_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_CLOSE_gpio) == 0)
			{
                key_CLOSE_state = KEY_S2;
			}
			else
				key_CLOSE_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_CLOSE_gpio) == 0)
            {
				key_CLOSE_state = KEY_S3;
                key_CLOSE_short_press();                      //短按少于500ms处理程序
			}else
				key_CLOSE_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_CLOSE_gpio) == 0)
            {
				key_CLOSE_state = KEY_S3;
				press_CLOSE++;
				if((press_CLOSE>50) && (long_press_flag_CLOSE == false))        //仅执行一次长按操作
                {
                    key_CLOSE_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_CLOSE = true;
				}
			}
			else
            {
				key_CLOSE_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_CLOSE_gpio) == 0)
            {
				key_CLOSE_state = KEY_S2;
			}
            else
            {
				key_CLOSE_state = KEY_S1;
            }
            press_CLOSE = 0;
            long_press_flag_CLOSE = false;
			break;

		default:
			key_CLOSE_state = KEY_S1;
			press_CLOSE = 0;
			break;
	}
}

void key_RUN_short_press()
{
    led_RUNL(ON);
    lift_run();
}

void key_RUN_long_press()
{
    led_RUNL(OFF);
}

void key_RUN_scan(void)
{
    static enum key_states_e key_RUN_state = KEY_S1;        //按键I/O状态
    static int32_t press_RUN = 0;                           //按键长按计数
    static int long_press_flag_RUN = false;                     //长按函数执行标志，只执行一次
    
    switch(key_RUN_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_RUN_gpio) == 0)
			{
                key_RUN_state = KEY_S2;
			}
			else
				key_RUN_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_RUN_gpio) == 0)
            {
				key_RUN_state = KEY_S3;
                key_RUN_short_press();                      //短按少于500ms处理程序
			}else
				key_RUN_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_RUN_gpio) == 0)
            {
				key_RUN_state = KEY_S3;
				press_RUN++;
				if((press_RUN>50) && (long_press_flag_RUN == false))        //仅执行一次长按操作
                {
                    key_RUN_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_RUN = true;
				}
			}
			else
            {
				key_RUN_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_RUN_gpio) == 0)
            {
				key_RUN_state = KEY_S2;
			}
            else
            {
				key_RUN_state = KEY_S1;
            }
            press_RUN = 0;
            long_press_flag_RUN = false;
			break;

		default:
			key_RUN_state = KEY_S1;
			press_RUN = 0;
			break;
	}
}

void key_STOP_short_press()
{
    led_STOPL(ON);
    lift_stop();
}

void key_STOP_long_press()
{
    led_STOPL(OFF);
}

void key_STOP_scan(void)
{
    static enum key_states_e key_STOP_state = KEY_S1;        //按键I/O状态
    static int32_t press_STOP = 0;                           //按键长按计数
    static int long_press_flag_STOP = false;                     //长按函数执行标志，只执行一次
    
    switch(key_STOP_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_STOP_gpio) == 0)
			{
                key_STOP_state = KEY_S2;
			}
			else
				key_STOP_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_STOP_gpio) == 0)
            {
				key_STOP_state = KEY_S3;
                key_STOP_short_press();                      //短按少于500ms处理程序
			}else
				key_STOP_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_STOP_gpio) == 0)
            {
				key_STOP_state = KEY_S3;
				press_STOP++;
				if((press_STOP>50) && (long_press_flag_STOP == false))        //仅执行一次长按操作
                {
                    key_STOP_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_STOP = true;
				}
			}
			else
            {
				key_STOP_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_STOP_gpio) == 0)
            {
				key_STOP_state = KEY_S2;
			}
            else
            {
				key_STOP_state = KEY_S1;
            }
            press_STOP = 0;
            long_press_flag_STOP = false;
			break;

		default:
			key_STOP_state = KEY_S1;
			press_STOP = 0;
			break;
	}
}

void key_4DS_short_press()
{
    led_4DL(ON);
    floor_penal[0][3] = 1;
}

void key_4DS_long_press()
{
    led_4DL(OFF);
    floor_penal[0][3] = 0;
}

void key_4DS_scan(void)
{
    static enum key_states_e key_4DS_state = KEY_S1;        //按键I/O状态
    static int32_t press_4DS = 0;                           //按键长按计数
    static int long_press_flag_4DS = false;                     //长按函数执行标志，只执行一次
    
    switch(key_4DS_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_4DS_gpio) == 0)
			{
                key_4DS_state = KEY_S2;
			}
			else
				key_4DS_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_4DS_gpio) == 0)
            {
				key_4DS_state = KEY_S3;
                key_4DS_short_press();                      //短按少于500ms处理程序
			}else
				key_4DS_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_4DS_gpio) == 0)
            {
				key_4DS_state = KEY_S3;
				press_4DS++;
				if((press_4DS>50) && (long_press_flag_4DS == false))        //仅执行一次长按操作
                {
                    key_4DS_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_4DS = true;
				}
			}
			else
            {
				key_4DS_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_4DS_gpio) == 0)
            {
				key_4DS_state = KEY_S2;
			}
            else
            {
				key_4DS_state = KEY_S1;
            }
            press_4DS = 0;
            long_press_flag_4DS = false;
			break;

		default:
			key_4DS_state = KEY_S1;
			press_4DS = 0;
			break;
	}
}

void key_3DS_short_press()
{
    led_3DL(ON);
    floor_penal[0][2] = 1;
//    lift_motor_down();
}

void key_3DS_long_press()
{
    led_3DL(OFF);
    floor_penal[0][2] = 0;
}

void key_3DS_scan(void)
{
    static enum key_states_e key_3DS_state = KEY_S1;        //按键I/O状态
    static int32_t press_3DS = 0;                           //按键长按计数
    static int long_press_flag_3DS = false;                     //长按函数执行标志，只执行一次
    
    switch(key_3DS_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_3DS_gpio) == 0)
			{
                key_3DS_state = KEY_S2;
			}
			else
				key_3DS_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_3DS_gpio) == 0)
            {
				key_3DS_state = KEY_S3;
                key_3DS_short_press();                      //短按少于500ms处理程序
			}else
				key_3DS_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_3DS_gpio) == 0)
            {
				key_3DS_state = KEY_S3;
				press_3DS++;
				if((press_3DS>50) && (long_press_flag_3DS == false))        //仅执行一次长按操作
                {
                    key_3DS_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_3DS = true;
				}
			}
			else
            {
				key_3DS_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_3DS_gpio) == 0)
            {
				key_3DS_state = KEY_S2;
			}
            else
            {
				key_3DS_state = KEY_S1;
            }
            press_3DS = 0;
            long_press_flag_3DS = false;
			break;

		default:
			key_3DS_state = KEY_S1;
			press_3DS = 0;
			break;
	}
}

void key_3US_short_press()
{
    led_3UL(ON);
    floor_penal[1][2] = 1;
//    lift_motor_up();
}

void key_3US_long_press()
{
    led_3UL(OFF);
    floor_penal[1][2] = 0;
}

void key_3US_scan(void)
{
    static enum key_states_e key_3US_state = KEY_S1;        //按键I/O状态
    static int32_t press_3US = 0;                           //按键长按计数
    static int long_press_flag_3US = false;                     //长按函数执行标志，只执行一次
    
    switch(key_3US_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_3US_gpio) == 0)
			{
                key_3US_state = KEY_S2;
			}
			else
				key_3US_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_3US_gpio) == 0)
            {
				key_3US_state = KEY_S3;
                key_3US_short_press();                      //短按少于500ms处理程序
			}else
				key_3US_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_3US_gpio) == 0)
            {
				key_3US_state = KEY_S3;
				press_3US++;
				if((press_3US>50) && (long_press_flag_3US == false))        //仅执行一次长按操作
                {
                    key_3US_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_3US = true;
				}
			}
			else
            {
				key_3US_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_3US_gpio) == 0)
            {
				key_3US_state = KEY_S2;
			}
            else
            {
				key_3US_state = KEY_S1;
            }
            press_3US = 0;
            long_press_flag_3US = false;
			break;

		default:
			key_3US_state = KEY_S1;
			press_3US = 0;
			break;
	}
}

void key_2DS_short_press()
{
    led_2DL(ON);
    floor_penal[0][1] = 1;
}

void key_2DS_long_press()
{
    led_2DL(OFF);
    floor_penal[0][1] = 0;
}

void key_2DS_scan(void)
{
    static enum key_states_e key_2DS_state = KEY_S1;        //按键I/O状态
    static int32_t press_2DS = 0;                           //按键长按计数
    static int long_press_flag_2DS = false;                     //长按函数执行标志，只执行一次
    
    switch(key_2DS_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_2DS_gpio) == 0)
			{
                key_2DS_state = KEY_S2;
			}
			else
				key_2DS_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_2DS_gpio) == 0)
            {
				key_2DS_state = KEY_S3;
                key_2DS_short_press();                      //短按少于500ms处理程序
			}else
				key_2DS_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_2DS_gpio) == 0)
            {
				key_2DS_state = KEY_S3;
				press_2DS++;
				if((press_2DS>50) && (long_press_flag_2DS == false))        //仅执行一次长按操作
                {
                    key_2DS_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_2DS = true;
				}
			}
			else
            {
				key_2DS_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_2DS_gpio) == 0)
            {
				key_2DS_state = KEY_S2;
			}
            else
            {
				key_2DS_state = KEY_S1;
            }
            press_2DS = 0;
            long_press_flag_2DS = false;
			break;

		default:
			key_2DS_state = KEY_S1;
			press_2DS = 0;
			break;
	}
}

void key_2US_short_press()
{
    led_2UL(ON);
    floor_penal[1][1] = 1;
}

void key_2US_long_press()
{
    led_2UL(OFF);
    floor_penal[1][1] = 0;
}

void key_2US_scan(void)
{
    static enum key_states_e key_2US_state = KEY_S1;        //按键I/O状态
    static int32_t press_2US = 0;                           //按键长按计数
    static int long_press_flag_2US = false;                     //长按函数执行标志，只执行一次
    
    switch(key_2US_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_2US_gpio) == 0)
			{
                key_2US_state = KEY_S2;
			}
			else
				key_2US_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_2US_gpio) == 0)
            {
				key_2US_state = KEY_S3;
                key_2US_short_press();                      //短按少于500ms处理程序
			}else
				key_2US_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_2US_gpio) == 0)
            {
				key_2US_state = KEY_S3;
				press_2US++;
				if((press_2US>50) && (long_press_flag_2US == false))        //仅执行一次长按操作
                {
                    key_2US_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_2US = true;
				}
			}
			else
            {
				key_2US_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_2US_gpio) == 0)
            {
				key_2US_state = KEY_S2;
			}
            else
            {
				key_2US_state = KEY_S1;
            }
            press_2US = 0;
            long_press_flag_2US = false;
			break;

		default:
			key_2US_state = KEY_S1;
			press_2US = 0;
			break;
	}
}

void key_1US_short_press()
{
    led_1UL(ON);
    floor_penal[1][0] = 1;
}

void key_1US_long_press()
{
    led_1UL(OFF);
    floor_penal[1][0] = 0;
}

void key_1US_scan(void)
{
    static enum key_states_e key_1US_state = KEY_S1;        //按键I/O状态
    static int32_t press_1US = 0;                           //按键长按计数
    static int long_press_flag_1US = false;                     //长按函数执行标志，只执行一次
    
    switch(key_1US_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_1US_gpio) == 0)
			{
                key_1US_state = KEY_S2;
			}
			else
				key_1US_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_1US_gpio) == 0)
            {
				key_1US_state = KEY_S3;
                key_1US_short_press();                      //短按少于500ms处理程序
			}else
				key_1US_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_1US_gpio) == 0)
            {
				key_1US_state = KEY_S3;
				press_1US++;
				if((press_1US>50) && (long_press_flag_1US == false))        //仅执行一次长按操作
                {
                    key_1US_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_1US = true;
				}
			}
			else
            {
				key_1US_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_1US_gpio) == 0)
            {
				key_1US_state = KEY_S2;
			}
            else
            {
				key_1US_state = KEY_S1;
            }
            press_1US = 0;
            long_press_flag_1US = false;
			break;

		default:
			key_1US_state = KEY_S1;
			press_1US = 0;
			break;
	}
}

void key_DOPEN_short_press()
{
    door_opened = 1;
}

void key_DOPEN_long_press()
{
    
}

void key_DOPEN_scan(void)
{
    static enum key_states_e key_DOPEN_state = KEY_S1;        //按键I/O状态
    static int32_t press_DOPEN = 0;                           //按键长按计数
    static int long_press_flag_DOPEN = false;                     //长按函数执行标志，只执行一次
    
    switch(key_DOPEN_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_DOPEN_gpio) == 0)
			{
                key_DOPEN_state = KEY_S2;
			}
			else
				key_DOPEN_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_DOPEN_gpio) == 0)
            {
				key_DOPEN_state = KEY_S3;
                key_DOPEN_short_press();                      //短按少于500ms处理程序
			}else
				key_DOPEN_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_DOPEN_gpio) == 0)
            {
				key_DOPEN_state = KEY_S3;
				press_DOPEN++;
				if((press_DOPEN>50) && (long_press_flag_DOPEN == false))        //仅执行一次长按操作
                {
                    key_DOPEN_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_DOPEN = true;
				}
			}
			else
            {
				key_DOPEN_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_DOPEN_gpio) == 0)
            {
				key_DOPEN_state = KEY_S2;
			}
            else
            {
				key_DOPEN_state = KEY_S1;
            }
            press_DOPEN = 0;
            long_press_flag_DOPEN = false;
            door_opened = 0;
			break;

		default:
			key_DOPEN_state = KEY_S1;
			press_DOPEN = 0;
            door_opened = 0;
			break;
	}
}

void key_DCLOSE_short_press()
{
    door_closed = 1;
}

void key_DCLOSE_long_press()
{
    
}

void key_DCLOSE_scan(void)
{
    static enum key_states_e key_DCLOSE_state = KEY_S1;        //按键I/O状态
    static int32_t press_DCLOSE = 0;                           //按键长按计数
    static int long_press_flag_DCLOSE = false;                     //长按函数执行标志，只执行一次
    
    switch(key_DCLOSE_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_DCLOSE_gpio) == 0)
			{
                key_DCLOSE_state = KEY_S2;
			}
			else
				key_DCLOSE_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_DCLOSE_gpio) == 0)
            {
				key_DCLOSE_state = KEY_S3;
                key_DCLOSE_short_press();                      //短按少于500ms处理程序
			}else
				key_DCLOSE_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_DCLOSE_gpio) == 0)
            {
				key_DCLOSE_state = KEY_S3;
				press_DCLOSE++;
				if((press_DCLOSE>50) && (long_press_flag_DCLOSE == false))        //仅执行一次长按操作
                {
                    key_DCLOSE_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_DCLOSE = true;
				}
			}
			else
            {
				key_DCLOSE_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_DCLOSE_gpio) == 0)
            {
				key_DCLOSE_state = KEY_S2;
			}
            else
            {
				key_DCLOSE_state = KEY_S1;
            }
            press_DCLOSE = 0;
            long_press_flag_DCLOSE = false;
            door_closed = 0;
			break;

		default:
			key_DCLOSE_state = KEY_S1;
			press_DCLOSE = 0;
            door_closed = 0;
			break;
	}
}

void key_DFLOOR_short_press()
{
    if( RUN == lift_state )
    {
        if( up_down_flag == UP )
        {
            lift_floor += 1;
        }
        else if ( up_down_flag == DOWN )
        {
            lift_floor -= 1;
        }
    }
}

void key_DFLOOR_long_press()
{
    
}

void key_DFLOOR_scan(void)
{
    static enum key_states_e key_DFLOOR_state = KEY_S1;        //按键I/O状态
    static int32_t press_DFLOOR = 0;                           //按键长按计数
    static int long_press_flag_DFLOOR = false;                     //长按函数执行标志，只执行一次
    
    switch(key_DFLOOR_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_DFLOOR_gpio) == 0)
			{
                key_DFLOOR_state = KEY_S2;
			}
			else
				key_DFLOOR_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_DFLOOR_gpio) == 0)
            {
				key_DFLOOR_state = KEY_S3;
                key_DFLOOR_short_press();                      //短按少于500ms处理程序
			}else
				key_DFLOOR_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_DFLOOR_gpio) == 0)
            {
				key_DFLOOR_state = KEY_S3;
				press_DFLOOR++;
				if((press_DFLOOR>50) && (long_press_flag_DFLOOR == false))        //仅执行一次长按操作
                {
                    key_DFLOOR_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_DFLOOR = true;
				}
			}
			else
            {
				key_DFLOOR_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_DFLOOR_gpio) == 0)
            {
				key_DFLOOR_state = KEY_S2;
			}
            else
            {
				key_DFLOOR_state = KEY_S1;
            }
            press_DFLOOR = 0;
            long_press_flag_DFLOOR = false;
			break;

		default:
			key_DFLOOR_state = KEY_S1;
			press_DFLOOR = 0;
			break;
	}
}

void key_DROUND_short_press()
{
    door_ground_flag = 1;       //电梯到最底层
}

void key_DROUND_long_press()
{
    
}

void key_DROUND_scan(void)
{
    static enum key_states_e key_DROUND_state = KEY_S1;        //按键I/O状态
    static int32_t press_DROUND = 0;                           //按键长按计数
    static int long_press_flag_DROUND = false;                     //长按函数执行标志，只执行一次
    
    switch(key_DROUND_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(key_DROUND_gpio) == 0)
			{
                key_DROUND_state = KEY_S2;
			}
			else
				key_DROUND_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(key_DROUND_gpio) == 0)
            {
				key_DROUND_state = KEY_S3;
                key_DROUND_short_press();                      //短按少于500ms处理程序
			}else
				key_DROUND_state = KEY_S1;
			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(key_DROUND_gpio) == 0)
            {
				key_DROUND_state = KEY_S3;
				press_DROUND++;
				if((press_DROUND>50) && (long_press_flag_DROUND == false))        //仅执行一次长按操作
                {
                    key_DROUND_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_DROUND = true;
				}
			}
			else
            {
				key_DROUND_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(key_DROUND_gpio) == 0)
            {
				key_DROUND_state = KEY_S2;
			}
            else
            {
				key_DROUND_state = KEY_S1;
            }
            press_DROUND = 0;
            door_ground_flag = 0;
            long_press_flag_DROUND = false;
			break;

		default:
			key_DROUND_state = KEY_S1;
			press_DROUND = 0;
            door_ground_flag = 0;
			break;
	}
}


void key_scan()
{
    if(STOP == lift_state)
    {
        key_RUN_scan();
    }
    if(RUN == lift_state)
    {
        key_1SW_scan();
        key_2SW_scan();
        key_3SW_scan();
        key_4SW_scan();
        key_OPEN_scan();
        key_CLOSE_scan();
        key_RUN_scan();
        key_STOP_scan();
        key_4DS_scan();
        key_3DS_scan();
        key_3US_scan();
        key_2DS_scan();
        key_2US_scan();
        key_1US_scan();
        key_DOPEN_scan();
        key_DCLOSE_scan();
        key_DFLOOR_scan();
        key_DROUND_scan();
    }
}
