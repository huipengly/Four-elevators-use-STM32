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
int32_t RUN = 0;
int32_t STOP = 0;
int32_t OPEN = 0;
int32_t OPENED = 0;
int32_t CLOSE = 0;
int32_t CLOSED = 0;
int32_t lift_floor = 0;
int32_t up_down_flag = 0;           //0代表下降，1代表上升，2代表停止。
int32_t up_flag = 0;                //电梯向上走
int32_t down_flag = 0;

void lift()
{
    int32_t floor_detect = 0;
    
    if(up_down_flag == UP)
    {
        up_down_flag = DOWN;                           //先改为下降，若之后的判断有需要上升的条件，则上升。
        for(floor_detect = lift_floor; floor_detect <= 4; floor_detect++)
        {
            if(floor_penal[0][floor_detect] == 1)       //向上一层是否有人
            {
                up_down_flag = UP;                     //电梯向上走
                break;
            }
            if(lift_penal[floor_detect] == 1)           //电梯里是否有人要在上一层下
            {
                up_down_flag = UP;                     //电梯向上走
                break;
            }
        }
        if(up_down_flag == DOWN)
        {
            up_down_flag = 2;//电机停止，下一次扫描下降时，若有下降请求，则电梯会下降，否则电梯停在当前层。
        }
        //motor(up_down_flag);传输给电机要做的事情。
    }
    else if(up_down_flag == DOWN)
    {
        up_down_flag = UP;                              //先改为上升，若之后的判断有需要下降的调节，则下降。
        for(floor_detect = lift_floor; floor_detect > 2; floor_detect--)
        {
            if(floor_penal[1][floor_detect-2] == 1)     //向下一层是否有人
            {
                up_down_flag = DOWN;                   //电梯向下走
                break;
            }
            if(lift_penal[floor_detect-2] == 1)         //电梯里是否有人要在下一层下
            {
                up_down_flag = DOWN;                   //电梯向上走
                break;
            }
        }
        if(up_down_flag == UP)
        {
            up_down_flag = 2;//电机停止，下一次扫描下降时，若有下降请求，则电梯会下降，否则电梯停在当前层。
        }
        //motor(up_down_flag);传输给电机要做的事情。
    }
}

void lift_door()
{
    
}

void lift_display()
{
    
}
