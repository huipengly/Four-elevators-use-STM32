/************************************************************
  Copyright (C), 刘慧鹏
  FileName: usart.c
  Author:刘慧鹏    Version :1.0    Date:2016/11/28
  Description:串口通信               // 模块描述     
  Version:使用printf串口通信，中文误码率高，不要发中文。
  Function List:   // 主要函数及其功能
    1. 
  History:         // 历史修改记录
      <author>  <time>   <version>   <desc>
      David    96/10/12     1.0     build this moudle 
***********************************************************/

#include "include.h"

void usart1_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    
    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure); 
    USART_Cmd(USART1, ENABLE);
}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(USART1, (uint8_t) ch);
    
    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		

    return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
    /* 等待串口1输入数据 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART1);
}

/*发送给上位机电梯数据
格式：开始标志#  运行 停止 1上 2上 2下 3上 3下 4下 1 2 3 4 
      开电梯门 电梯门开启 关电梯门 电梯门关闭 up dowm arrive_flag  电梯层数 结束标志$;
*/
extern int32_t lift_state;              //电梯运行状态,RUN/STOP
extern int32_t lift_penal[4];           //1SW/2SW/3SW/4SW
extern int32_t floor_penal[2][4];       //1DS/2DS/3DS/4DS
                                        //1US/2US/3US/4US
extern int32_t door_open;               //开门信号    
extern int32_t door_opened;             //门开启信号    
extern int32_t door_close;              //关门信号
extern int32_t door_closed;             //门关闭信号
extern int32_t up_down_flag;            //0代表下降，1代表上升，2代表停止。
extern int32_t lift_floor;              //记录当前电梯楼层
void send_state_to_pc(void)
{
    printf("# ");
    printf("%d ", lift_state);              //RUN
    printf("%d ", !lift_state);             //STOP    
    printf("%d ", floor_penal[1][0]);       //1US
    printf("%d ", floor_penal[1][1]);       //2US
    printf("%d ", floor_penal[0][1]);       //2DS
    printf("%d ", floor_penal[1][2]);       //3US
    printf("%d ", floor_penal[0][2]);       //3DS
    printf("%d ", floor_penal[0][3]);       //4DS
    printf("%d ", lift_penal[0]);           //1SW
    printf("%d ", lift_penal[1]);           //2SW
    printf("%d ", lift_penal[2]);           //3SW
    printf("%d ", lift_penal[3]);           //4SW
    printf("%d ", door_open);               //OPEN
    printf("%d ", door_opened);             //OPEND
    printf("%d ", door_close);              //CLOSE
    printf("%d ", door_closed);             //CLOSED
    if(UP == up_down_flag)
    {
        printf("%d ", 1);                   //UP
    }
    else
    {
        printf("%d ", 0);
    }
    printf("%d ", !up_down_flag);           //DOWN
    if(WAIT == up_down_flag)
    {
        printf("%d ", 1);                   //WAIT
    }
    else
    {
        printf("%d ", 0);
    }
    printf("%d ", lift_floor);              //floor
    printf("$");
}
