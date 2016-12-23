/************************************************************
  Copyright (C), 刘慧鹏
  FileName: key.c
  Author:刘慧鹏    Version :2.0    Date:2016/12/12
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
***********************************************************/

#include "include.h"

extern int32_t lift_penal[4];       //1SW/2SW/3SW/4SW
extern int32_t floor_penal[2][4];   //1DS/2DS/3DS/4DS
                                    //1US/2US/3US/4US
//extern int32_t state_penal[4];      //OPEN/CLOSE/RUN/STOP
//extern int32_t RUN;
//extern int32_t STOP;
extern int32_t OPEN;
extern int32_t OPENED;
extern int32_t CLOSE;
extern int32_t CLOSED;
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

void key_scan()
{
	static enum key_states_e keyc_state=KEY_S1;         //GPIOC组IO状态
	static enum key_states_e key_dfloor_state=KEY_S1;   //dfloor状态    
	static enum key_states_e key_dclose_state=KEY_S1;   //dclose状态
	static int pressc = 0;                              //GPIOC组IO长按计数
    static int press_dfloor = 0;                        //dfloor长按计数
    static int press_dclose = 0;                        //dclose长按计数
    static uint32_t keyc_value_S1 = 0;                  //记录GPIOC组IO状态
    static bool long_press_flagc = false;               //长按函数执行标志，只执行一次
    static bool long_press_flag_dfloor = false;
    static bool long_press_flag_dclose = false;

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
    
    /*GPIOE.7 dfloor处理。接近开关处理，通常状态为1，接近金属为0*/
    switch(key_dfloor_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 0)
			{
                key_dfloor_state = KEY_S2;
			}
			else
				key_dfloor_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 0)
            {
				key_dfloor_state = KEY_S3;
                key_dfloor_short_press();                      //短按少于500ms处理程序
			}else
				key_dfloor_state = KEY_S1;

			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 0)
            {
				key_dfloor_state = KEY_S3;
				press_dfloor++;
				if((press_dfloor>50) && (long_press_flag_dfloor == false))        //仅执行一次长按操作
                {
                    key_dfloor_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_dfloor = true;
				}
			}
			else
            {
				key_dfloor_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) == 0)
            {
				key_dfloor_state = KEY_S2;
			}
            else
            {
				key_dfloor_state = KEY_S1;
            }
            press_dfloor = 0;
            long_press_flag_dfloor = false;
			break;

		default:
			key_dfloor_state = KEY_S1;
			press_dfloor = 0;
			break;
	}
    
    /*GPIOA.4 dclose处理。接触开关处理，通常状态为1，闭合为0*/
    switch(key_dclose_state)
	{
		case KEY_S1:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
			{
                key_dclose_state = KEY_S2;
			}
			else
				key_dclose_state = KEY_S1;
			break;

		case KEY_S2:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
            {
				key_dclose_state = KEY_S3;
                key_dclose_short_press();                      //短按少于500ms处理程序
			}else
				key_dclose_state = KEY_S1;

			break;

		case KEY_S3:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
            {
				key_dclose_state = KEY_S3;
				press_dclose++;
				if((press_dclose>50) && (long_press_flag_dclose == false))        //仅执行一次长按操作
                {
                    key_dclose_long_press();                   //长按500ms处理程序,BUG：长按每次扫描都要执行长按程序，之后改
                    long_press_flag_dclose = true;
				}
			}
			else
            {
				key_dclose_state = KEY_S4;
            }

			break;

		case KEY_S4:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
            {
				key_dclose_state = KEY_S2;
			}
            else
            {
				key_dclose_state = KEY_S1;
            }
            press_dclose = 0;
            long_press_flag_dclose = false;
			break;

		default:
			key_dclose_state = KEY_S1;
			press_dclose = 0;
			break;
	}
}

void keyc_short_press(uint32_t key_value)
{
    if( RUN == lift_state )
    {
        switch(key_value)
        {
            case key_1SW:
                led_1SWL(ON);
                lift_penal[0] = 1;
                break;
            case key_2SW:
                led_2SWL(ON);
                lift_penal[1] = 1;
                break;
            case key_3SW:
                led_3SWL(ON);
                lift_penal[2] = 1;
                break;
            case key_4SW:
                led_4SWL(ON);
                lift_penal[3] = 1;
                break;
            case key_OPEN:
                led_OPENL(ON);
                OPEN = 1;
                break;
            case key_CLOSE:
                led_CLOSEL(ON);
                CLOSE = 1;
                break;
            case key_STOP:
                lift_stop();
                break;
            case key_4DS:
                led_4DL(ON);
                floor_penal[0][3] = 1;
                break;
            case key_3DS:
                led_3DL(ON);
                floor_penal[0][2] = 1;
            lift_motor_down();
                break;
            case key_3US:
                led_3UL(ON);
                floor_penal[1][2] = 1;
            lift_motor_up();
                break;
            case key_2DS:
                led_2DL(ON);
                floor_penal[0][1] = 1;
                break;
            case key_2US:
                led_2UL(ON);
                floor_penal[1][1] = 1;
                break;
            case key_1US:
                led_1UL(ON);
                floor_penal[1][0] = 1;
                break;
            case key_DROUND:
                lift_floor = 0;
                lift_init_ok = 1;
                break;
            case key_DOPEN:
                OPENED = 1;
                break;
            default:
                break;
        }
    }
    else if( (STOP == lift_state) && (key_RUN == key_value))
    {
        lift_run();
    }
}

void keyc_long_press(uint32_t key_value)
{
    switch(key_value)
    {
        case key_1SW:
            led_1SWL(OFF);
            lift_penal[0] = 0;
            break;
        case key_2SW:
            led_2SWL(OFF);
            lift_penal[1] = 0;
            break;
        case key_3SW:
            led_3SWL(OFF);
            lift_penal[2] = 0;
            break;
        case key_4SW:
            led_4SWL(OFF);
            lift_penal[3] = 0;
            break;
        case key_OPEN:
            led_OPENL(OFF);
            OPEN = 0;
            break;
        case key_CLOSE:
            led_CLOSEL(OFF);
            CLOSE = 0;
            break;
//        case key_RUN:
//            led_RUNL(OFF);
//            RUN = 0;
//            break;
//        case key_STOP:
//            led_STOPL(OFF);
//            STOP = 0;
//            break;
        case key_4DS:
            led_4DL(OFF);
            floor_penal[0][3] = 0;
            break;
        case key_3DS:
            led_3DL(OFF);
            floor_penal[0][2] = 0;
            break;
        case key_3US:
            led_3UL(OFF);
            floor_penal[1][2] = 0;
            break;
        case key_2DS:
            led_2DL(OFF);
            floor_penal[0][1] = 0;
            break;
        case key_2US:
            led_2UL(OFF);
            floor_penal[1][1] = 0;
            break;
        case key_1US:
            led_1UL(OFF);
            floor_penal[1][0] = 0;
            break;
//        case key_DROUND:
//            lift_floor = 0;
//            break;
//        case key_DOPEN:
//            OPENED = 0;
//            break;
        default:
            break;
    }
}

void key_dfloor_short_press()        //平层开关
{
    if( RUN == lift_state )
    {
        if( up_down_flag == UP )
        {
            lift_floor += 1;
            lift_penal[lift_floor-1] = 0;
            switch(lift_floor)
            {
                case 1:
                    led_1SWL(OFF);
                    break;
                case 2:
                    led_2SWL(OFF);
                    break;
                case 3:
                    led_3SWL(OFF);
                    break;
                case 4:
                    led_4SWL(OFF);
                    break;
            }
        }
        else if ( up_down_flag == DOWN )
        {
            lift_floor -= 1;
            lift_penal[lift_floor-1] = 0;
            switch(lift_floor)
            {
                case 1:
                    led_1SWL(OFF);
                    break;
                case 2:
                    led_2SWL(OFF);
                    break;
                case 3:
                    led_3SWL(OFF);
                    break;
                case 4:
                    led_4SWL(OFF);
                    break;
            }
        }
    }
//    led_3SWL(ON);
}

void key_dfloor_long_press()
{
    
}

void key_dclose_short_press()        //DCLOSE
{
    CLOSED = 1;
//    led_3SWL(ON);
}

void key_dclose_long_press()
{
//    led_3SWL(OFF);
}
