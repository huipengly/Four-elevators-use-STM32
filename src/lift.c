/************************************************************
  Copyright (C), 刘慧鹏
  FileName: lift.c
  Author:刘慧鹏    Version :1.0    Date:2016/12/03
  Description:电梯处理程序               // 模块描述     
  Version:电梯逻辑处理
  Function List:   // 主要函数及其功能
    1. 
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

int32_t lift_penal[4] = {0};        //1SW/2SW/3SW/4SW
int32_t floor_penal[2][4] = {0};    //1DS/2DS/3DS/4DS
                                    //1US/2US/3US/4US
//int32_t state_penal[4] = {0};       //OPEN/CLOSE/RUN/STOP
//int32_t RUN = 0;
//int32_t STOP = 0;
int32_t lift_stay = 0;
int32_t door_open = 0;              //开门信号    
int32_t door_opened = 0;            //门开启信号    
int32_t door_close = 0;             //关门信号
int32_t door_closed = 0;            //门关闭信号
int32_t door_ground_flag = 0;       //轿厢是否到最底层
int32_t pre_lift_floor = 0;         //记录上一次电梯楼层
int32_t lift_floor = 0;             //记录当前电梯楼层
int32_t pre_up_down_flag = 0;       //记录上一次的上升下降标志量
int32_t up_down_flag = 0;           //0代表下降，1代表上升，2代表停止。
//int32_t up_flag = 0;                //电梯向上走
//int32_t down_flag = 0;
//int32_t lift_init_ok = 0;           //电梯初始化标志位，0表示还未降到最底层，1表示降到最底层，初始化完成
int32_t lift_state = STOP;             //电梯运行状态,RUN/STOP

void lift(void)
{
    static enum lift_states_e lift_states = LIFT_stop;
    static uint32_t wait_time = 0;              //记录开门后的等待时间
    static uint32_t door_open_time = 0;         //记录开门用时，判断门是否卡住
    static uint32_t door_close_time = 0;        //记录关门用时，判断门是否卡住
    uint32_t value_wait = 0;
    uint32_t value_run = 0;
    
    if(STOP == lift_state)
    {
        lift_states = LIFT_stop;
    }
    else if(RUN == lift_state)
    {
        if((lift_floor >= 0) && (lift_floor <= 4))
        {
            display(lift_floor);
        }
        else 
        {
            hc595_write_byte(0);
        }
    }
    
    switch(lift_states)
    {
        case LIFT_stop:
            lift_motor_stop();
            if(RUN == lift_state)
            {
                lift_states = LIFT_init;
                hc595_enable();
            }
            break;
        case LIFT_init:                         //电梯初始化，向下运行
            lift_motor_down();
            up_down_flag = DOWN;       
            lift_states = LIFT_init_ground;
            break;
        case LIFT_init_ground:                  //如果到底层则电梯向上运行
            if(1 == door_ground_flag)
            {
                up_down_flag = UP;
                lift_floor = 0;
                lift_states = LIFT_init_1st;
            }
            break;
        case LIFT_init_1st:                     //到达第一层停止
            if(1 == lift_floor)
            {
                up_down_flag = WAIT;
                lift_states = LIFT_wait;
            }
            break;
        case LIFT_wait:                         //判断是否继续等待
//            if(WAIT == up_down_flag)
//            {
        
                if(1 == door_open)
                {
                    door_open = 0;
                    door_motor_open();
                    led_OPENL(OFF);
                    lift_states = LIFT_arrive_wait;
                }
                if(1 == lift_floor)
                {
                    if(1 == floor_penal[1][0])
                    {
                        floor_penal[1][0] = 0;
                        led_1UL(OFF);
                        lift_states = LIFT_arrive_stop;
                        break;
                    }
                    else if( 0 == floor_penal[1][0] )
                    {
                        value_wait = lift_penal[1] + lift_penal[2] + lift_penal[3];
                        value_wait += floor_penal[0][1] + floor_penal[0][2] + floor_penal[0][3];
                        value_wait += floor_penal[1][1] + floor_penal[1][2] + floor_penal[1][3];
                        if(0 != value_wait)                  //如果上面有至少有一个为1，则点电梯上升
                        {
                            up_down_flag = UP;
                            lift_states = LIFT_run;
                        }
                    }
                }
                else if(2 == lift_floor)
                {
                    if((1 == floor_penal[0][1]) || (1 == floor_penal[1][1]))
                    {
                        floor_penal[0][1] = 0;
                        floor_penal[1][1] = 0;
                        led_2UL(OFF);
                        led_2DL(OFF);
                        lift_states = LIFT_arrive_stop;
                        break;
                    }
                    else if((0 == floor_penal[0][1]) && (0 == floor_penal[1][1]))
                    {
                        value_wait = lift_penal[2] + lift_penal[3];
                        value_wait += floor_penal[0][2] + floor_penal[0][3];
                        value_wait += floor_penal[1][2] + floor_penal[1][3];
                        if(0 != value_wait)                  //如果上面有至少有一个为1，则点电梯上升
                        {
                            up_down_flag = UP;
                            lift_states = LIFT_run;
                        }
                        else if(0 == value_wait)             //如果无上升请求，则判断是否有下降请求
                        {
                            value_wait = lift_penal[0];
                            value_wait += floor_penal[0][0];
                            value_wait += floor_penal[1][0];
                            if(0 != value_wait)
                            {
                                up_down_flag = DOWN;
                                lift_states = LIFT_run;
                            }
                        }
                    }
                }
                else if(3 == lift_floor)
                {
                    if((1 == floor_penal[0][2]) || (1 == floor_penal[1][2]))
                    {
                        floor_penal[0][2] = 0;
                        floor_penal[1][2] = 0;
                        led_3UL(OFF);
                        led_3DL(OFF);
                        lift_states = LIFT_arrive_stop;
                        break;
                    }
                    else if((0 == floor_penal[0][2]) && (0 == floor_penal[1][2]))
                    {
                        value_wait = lift_penal[3];
                        value_wait += floor_penal[0][3];
                        value_wait += floor_penal[1][3];
                        if(0 != value_wait)                  //如果上面有至少有一个为1，则点电梯上升
                        {
                            up_down_flag = UP;
                            lift_states = LIFT_run;
                        }
                        else if(0 == value_wait)             //如果无上升请求，则判断是否有下降请求
                        {
                            value_wait = lift_penal[0] + lift_penal[1];
                            value_wait += floor_penal[0][0] + floor_penal[0][1];
                            value_wait += floor_penal[1][0] + floor_penal[1][1];
                            if(0 != value_wait)
                            {
                                up_down_flag = DOWN;
                                lift_states = LIFT_run;
                            }
                        }
                    }
                }
                else if(4 == lift_floor)
                {
                    if(1 == floor_penal[0][3])
                    {
                        floor_penal[0][3] = 0;
                        led_4DL(OFF);
                        lift_states = LIFT_arrive_stop;
                        break;
                    }
                    else if( 0 == floor_penal[0][3] )
                    {
                        value_wait = lift_penal[0] + lift_penal[1] + lift_penal[2];
                        value_wait += floor_penal[0][0] + floor_penal[0][1] + floor_penal[0][2];
                        value_wait += floor_penal[1][0] + floor_penal[1][1] + floor_penal[1][2];
                        if(0 != value_wait)                  //如果上面有至少有一个为1，则点电梯下降
                        {
                            up_down_flag = DOWN;
                            lift_states = LIFT_run;
                        }
                    }
                }
//            }
//            else if((UP == up_down_flag) || (DOWN == up_down_flag)
//            {
//                lift_states = LIFT_run;
//            }
            break;
        case LIFT_run:                          //判断电梯运行状态
            if (pre_lift_floor != lift_floor)   //楼层发生变化
            {
//                if((1 == lift_penal[lift_floor-1]) || (1 == floor_penal[0][lift_floor-1]) || (1 == floor_penal[1][lift_floor-1]))
//                {
//                    lift_states = LIFT_arrive_stop;         //到达有需求的楼层
//                }
                if(UP == up_down_flag)
                {
                    if(1 == lift_floor)
                    {
                        if(1 == lift_penal[0])          //如果有到1楼请求
                        {
                            lift_penal[0] = 0;          //清零请求
                            led_1SWL(OFF);
                            floor_penal[1][0] = 0;  //清零请求
                            led_1UL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[0])     //如果没有到1楼请求
                        {
                            if(1 == floor_penal[1][0])  //如果1US按下
                            {
                                floor_penal[1][0] = 0;  //清零请求
                                led_1UL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[1][0])
                            {
                                value_run = lift_penal[1] + lift_penal[2] + lift_penal[3];
                                value_run += floor_penal[1][1] + floor_penal[1][2] + floor_penal[1][3];
                                if(0 != value_run)
                                {
                                    up_down_flag = UP;              //电梯向上走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果上面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                    if(2 == lift_floor)
                    {
                        if(1 == lift_penal[1])          //如果有到2楼请求
                        {
                            lift_penal[1] = 0;          //清零请求
                            led_2SWL(OFF);
                            floor_penal[1][1] = 0;  //清零请求
                            led_2UL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[1])     //如果没有到2楼请求
                        {
                            if(1 == floor_penal[1][1])  //如果2US按下
                            {
                                floor_penal[1][1] = 0;  //清零请求
                                led_2UL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[1][1])//2US
                            {
                                value_run = lift_penal[2] + lift_penal[3];
                                value_run += floor_penal[1][2] + floor_penal[1][3];
                                if(0 != value_run)
                                {
                                    up_down_flag = UP;              //电梯向上走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果上面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                    if(3 == lift_floor)
                    {
                        if(1 == lift_penal[2])          //如果有到3楼请求
                        {
                            lift_penal[2] = 0;          //清零请求
                            led_3SWL(OFF);
                            floor_penal[1][2] = 0;  //清零请求
                            led_3UL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[2])     //如果没有到3楼请求
                        {
                            if(1 == floor_penal[1][2])  //如果3US按下
                            {
                                floor_penal[1][2] = 0;  //清零请求
                                led_3UL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[1][2])//2US
                            {
                                value_run = lift_penal[3];
                                value_run += floor_penal[0][3];
                                if(0 != value_run)
                                {
                                    up_down_flag = UP;              //电梯向上走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果上面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                    if(4 == lift_floor)
                    {
                        if(1 == lift_penal[3])          //如果有到4楼请求
                        {
                            lift_penal[0] = 0;
                            led_1SWL(OFF);
                            lift_penal[1] = 0;
                            led_2SWL(OFF);
                            lift_penal[2] = 0;
                            led_3SWL(OFF);
                            lift_penal[3] = 0;          //清零请求
                            led_4SWL(OFF);
                            floor_penal[0][3] = 0;
                            led_4DL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[3])     //如果没有到4楼请求,则清零楼层选择，向下运行
                        {
                            lift_penal[0] = 0;
                            led_1SWL(OFF);
                            lift_penal[1] = 0;
                            led_2SWL(OFF);
                            lift_penal[2] = 0;
                            led_3SWL(OFF);
                            lift_penal[3] = 0;          //清零请求
                            if(1 == floor_penal[0][3])  //如果4DS按下
                            {
                                floor_penal[0][3] = 0;
                                led_4DL(OFF);
                                lift_states = LIFT_arrive_stop;
                            }
                            else if(0 == floor_penal[0][3])
                            {
                                value_run = 0;
                                value_run += floor_penal[0][0] + floor_penal[0][1] + floor_penal[0][2];
                                value_run += floor_penal[1][0] + floor_penal[1][1] + floor_penal[1][2];
                                if(0 != value_run)          //判断楼下是否有请求，有则下降，没有则等待。
                                {
                                    up_down_flag = DOWN;
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;
                                }
                            }
                        }
                    }
                }
                else if(DOWN == up_down_flag)
                {
                    if(1 == lift_floor)
                    {
                        if(1 == lift_penal[0])          //如果有到1楼请求
                        {
                            lift_penal[0] = 0;
                            led_1SWL(OFF);
                            lift_penal[1] = 0;
                            led_2SWL(OFF);
                            lift_penal[2] = 0;
                            led_3SWL(OFF);
                            lift_penal[3] = 0;          //清零请求
                            led_4SWL(OFF);
                            floor_penal[1][0] = 0;
                            led_1UL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[0])     //如果没有到1楼请求,则清零楼层选择，向下运行
                        {
                            lift_penal[0] = 0;
                            led_1SWL(OFF);
                            lift_penal[1] = 0;
                            led_2SWL(OFF);
                            lift_penal[2] = 0;
                            led_3SWL(OFF);
                            lift_penal[3] = 0;          //清零请求
                            led_4SWL(OFF);
                            if(1 == floor_penal[1][0])  //如果1US按下
                            {
                                floor_penal[1][0] = 0;
                                led_1UL(OFF);
                                lift_states = LIFT_arrive_stop;
                            }
                            else if(0 == floor_penal[1][0])
                            {
                                value_run = 0;
                                value_run += floor_penal[0][1] + floor_penal[0][2] + floor_penal[0][3];
                                value_run += floor_penal[1][1] + floor_penal[1][2] + floor_penal[1][3];
                                if(0 != value_run)          //判断楼上是否有请求，有则上升，没有则等待。
                                {
                                    up_down_flag = UP;
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;
                                }
                            }
                        }
                    }
                    if(2 == lift_floor)
                    {
                        if(1 == lift_penal[1])          //如果有到2楼请求
                        {
                            lift_penal[1] = 0;          //清零请求
                            led_2SWL(OFF);
                            floor_penal[0][1] = 0;  //清零请求
                            led_2DL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[1])     //如果没有到2楼请求
                        {
                            if(1 == floor_penal[0][1])  //如果2DS按下
                            {
                                floor_penal[0][1] = 0;  //清零请求
                                led_2DL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[0][1])//2DS
                            {
                                value_run = lift_penal[0];
                                value_run += floor_penal[1][0];
                                if(0 != value_run)
                                {
                                    up_down_flag = DOWN;            //电梯向下走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果下面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                    if(3 == lift_floor)
                    {
                        if(1 == lift_penal[2])          //如果有到3楼请求
                        {
                            lift_penal[2] = 0;          //清零请求
                            led_3SWL(OFF);
                            floor_penal[0][2] = 0;  //清零请求
                            led_3DL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[2])     //如果没有到3楼请求
                        {
                            if(1 == floor_penal[0][2])  //如果3DS按下
                            {
                                floor_penal[0][2] = 0;  //清零请求
                                led_3DL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[0][2])//3DS
                            {
                                value_run = lift_penal[0] + lift_penal[1];
                                value_run += floor_penal[0][0] + floor_penal[0][1];
                                if(0 != value_run)
                                {
                                    up_down_flag = DOWN;            //电梯向下走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果下面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                    if(4 == lift_floor)
                    {
                        if(1 == lift_penal[3])          //如果有到4楼请求
                        {
                            lift_penal[3] = 0;          //清零请求
                            led_4SWL(OFF);
                            floor_penal[0][3] = 0;  //清零请求
                            led_4DL(OFF);
                            lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                        }
                        else if(0 == lift_penal[3])     //如果没有到4楼请求
                        {
                            if(1 == floor_penal[0][3])  //如果4DS按下
                            {
                                floor_penal[0][3] = 0;  //清零请求
                                led_4DL(OFF);
                                lift_states = LIFT_arrive_stop;
                                                        //跳转到电梯到达状态
                            }
                            else if(0 == floor_penal[0][3])//4DS
                            {
                                value_run = lift_penal[0] + lift_penal[1] + lift_penal[2];
                                value_run += floor_penal[0][0] + floor_penal[0][1] + floor_penal[0][2];
                                if(0 != value_run)
                                {
                                    up_down_flag = DOWN;            //电梯向下走
                                }
                                else if(0 == value_run)
                                {
                                    up_down_flag = WAIT;
                                    lift_states = LIFT_wait;        //如果下面没有楼层有请求，则停在当前层。
                                }
                            }
                        }
                    }
                }
                if(-1 == pre_lift_floor)
                {
                    if(pre_up_down_flag == up_down_flag)
                    {
                        pre_up_down_flag = WAIT;
                    }
                }
                pre_lift_floor = lift_floor;
            }
            else if(pre_lift_floor == lift_floor)//还没有楼层变化
            {
                
            }
            break;
        case LIFT_arrive_stop:                  //停止，开门
            lift_motor_stop();
            door_motor_open();
            lift_states = LIFT_arrive_wait;
            break;
        case LIFT_arrive_wait:                  //门开后等待2s，关门
            if(0 == door_opened)
            {
                ++door_open_time;
                if(10 == door_open_time)        //判断门是否卡住，1s
                {
                    //报警
                    //停止运行
                    door_open_time = 0;
                    led_STATE(ON);
                }
            }
            else if(1 == door_opened)           //判断是否完全打开
            {
                door_motor_stop();
                ++wait_time;
                //关门按下强行wait_time=20
                if(1 == door_close)
                {
                    door_close = 0;
                    led_CLOSEL(OFF);
                    wait_time = 20;
                }
                if(20 == wait_time)             //等待2s，关门
                {
                    wait_time = 0;
                    door_motor_close();
                    lift_states = LIFT_arrive_door_closed;
                }
            }
            break;
        case LIFT_arrive_door_closed:
            if(0 == door_closed)
            {
                ++door_close_time;
                if(10 == door_close_time)
                {
                    //报警
                    //停止运行
                    door_close_time = 0;
                    led_STATE(ON);
                }
                //开门，跳转会上一个状态
                if(1 == door_open)
                {
                    door_open = 0;
                    door_motor_open();
                    led_OPENL(OFF);
                    lift_states = LIFT_arrive_wait;
                }
            }
            if(1 == door_closed)
            {
                door_motor_stop();
                if((UP == up_down_flag) || (DOWN == up_down_flag))
                {
                    lift_states = LIFT_run;
                    pre_lift_floor = -1;            //强行造成run里到达新楼层
//                    pre_up_down_flag = WAIT;
                }
                else if(WAIT == up_down_flag)
                {
                    lift_states = LIFT_wait;
                }
            }
            break;
        default:
            if((UP == up_down_flag) || (DOWN == up_down_flag))
            {
                lift_states = LIFT_run;
            }
            else if(WAIT == up_down_flag)
            {
                lift_states = LIFT_wait;
            }
            break;
    }
    
    if((lift_states != LIFT_arrive_stop) && (lift_states != LIFT_arrive_wait) && (lift_states != LIFT_arrive_door_closed))
    {
        if(pre_up_down_flag != up_down_flag)
        {
            pre_up_down_flag = up_down_flag;
            switch(up_down_flag)
            {
                case DOWN:
                    lift_motor_down();
                    led_UL(OFF);
                    led_DL(ON);
                    led_LIGHT(ON);
                    break;
                case UP:
                    lift_motor_up();
                    led_UL(ON);
                    led_DL(OFF);
                    led_LIGHT(ON);
                    break;
                case WAIT:
                    lift_motor_stop();
                    led_UL(OFF);
                    led_DL(OFF);
                    led_LIGHT(OFF);
                    break;
            }
        }
    }
}


void lift_run(void)
{
    led_RUNL(ON);
    led_STOPL(OFF);
    lift_state = RUN;
}

void lift_stop(void)
{
    led_RUNL(OFF);
    led_STOPL(ON);
    lift_state = STOP;
    lift_motor_stop();
    door_motor_stop();
}
