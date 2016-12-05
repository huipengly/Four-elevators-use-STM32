/************************************************************
  Copyright (C), 刘慧鹏
  FileName: HC595.c
  Author:刘慧鹏    Version :1.0    Date:2016/12/04
  Description:HC595芯片驱动               // 模块描述
  Version:               // 版本信息
  Function List:   // 主要函数及其功能
    1. void hc595_init(void)                //HC595初始化
    2. void hc595_write_byte(uint8_t byte)  //HC595数据写入
    3. void display(int32_t display_floor)  //共阴数码管显示楼层
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

/*共阴数码管，A连接595的Q0口，DP连接Q7口*/
const uint8_t number_code[4] = {0x60, 0xda, 0xf2, 0x66};

void hc595_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = HC595_DATA|HC595_SCLK;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = HC595_RCLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, HC595_DATA|HC595_SCLK);
	GPIO_ResetBits(GPIOB, HC595_RCLK);
}

void hc595_write_byte(uint8_t byte)
{
	uint8_t i, num;
	num = byte;
	for (i=0; i<8; i++)
	{
		GPIO_ResetBits(GPIOA, HC595_SCLK);
		if ((num&0x01)==0x01)
			GPIO_SetBits(GPIOA, HC595_DATA);
		else
			GPIO_ResetBits(GPIOA, HC595_DATA);
        Delay_us(1);
		GPIO_SetBits(GPIOA, HC595_SCLK);
		num = num>>1;
	}
    GPIO_ResetBits(GPIOB, HC595_RCLK);
    GPIO_SetBits(GPIOB, HC595_RCLK);
}

void display(int32_t display_floor)
{
    hc595_write_byte(number_code[display_floor - 1]);
}
