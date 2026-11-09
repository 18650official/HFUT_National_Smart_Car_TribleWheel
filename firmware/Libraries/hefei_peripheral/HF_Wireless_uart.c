/********************************************************************************************************************
* @file            HF_Wireless_uart.c
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
********************************************************************************************************************/

#include "HF_Wireless_uart.h"

uint8 receiveSuccessFlag = 0;
char receiveDat; //串口回传的数据

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 发送数据
// @param       data            8bit 数据
// @return      uint32          剩余发送长度
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_putchar (uint8 data)
{
    uart_putchar(UART_6, data);                                 // 发送最后的数据

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 串口接收数据
// @param       data            8bit数据
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_getchar (uint8 data)
{
    uart_getchar(UART_6);
    return data;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 串口发送数据
// @param       buff            数据地址
// @param       len             数据长度
// @return      uint32_t        提示信息
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_putbuff ( uint8 *buff, uint32 len)
{

    uart_putbuff(UART_6, buff, len);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 发送字符串
// @param       *str            要发送的字符串地址
// @return      uint32          剩余发送长度
// Sample usage:                wireless_uart_send_string("hello");
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_putstr ( uint8 *str)
{

    uart_putstr(UART_6, str);

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块 发送摄像头图像至上位机查看图像
// @param       *image_addr     需要发送的图像地址
// @param       image_size      图像的大小
// @return      void
// Sample usage:                wireless_uart_send_image(&mt9v03x_image[0][0], MT9V03X_IMAGE_SIZE);
//-------------------------------------------------------------------------------------------------------------------
//uint8 wireless_uart_putimage ( uint8 *image_addr, uint32 image_size)
//{
//    extern uint8 camera_send_image_frame_header[4];
//    wireless_uart_send_buff(camera_send_image_frame_header, 4);
//    wireless_uart_send_buff((uint8 *)image_addr, image_size);
//
//    return 0 ;
//}



//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口模块初始化
// @param       void
// @return      void
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_init (void)
{

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //开启GPIOA的时钟
//
//    /*GPIO初始化*/
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //C0 TX设为输出
//
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //C1 RX设为输入
//
//    /*USART初始化*/
//    USART_InitTypeDef USART_InitStructure;                  //定义结构体变量
//    USART_InitStructure.USART_BaudRate = 115200;              //波特率
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制，不需要
//    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //模式，发送模式和接收模式均选择
//    USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶校验，不需要
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位，选择1位
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //字长，选择8位
//    USART_Init(UART6, &USART_InitStructure);               //将结构体变量交给USART_Init，配置USART1
//
//    /*中断输出配置*/
//    USART_ITConfig(UART6, USART_IT_RXNE, ENABLE);          //开启串口接收数据的中断
//
//    /*NVIC中断分组*/
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         //配置NVIC为分组2
//
//    /*NVIC配置*/
//    NVIC_InitTypeDef NVIC_InitStructure;                    //定义结构体变量
//    NVIC_InitStructure.NVIC_IRQChannel = UART6_IRQn;       //选择配置NVIC的USART1线
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //指定NVIC线路使能
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       //指定NVIC线路的抢占优先级为1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //指定NVIC线路的响应优先级为1
//    NVIC_Init(&NVIC_InitStructure);                         //将结构体变量交给NVIC_Init，配置NVIC外设
//
//    /*USART使能*/
//    USART_Cmd(UART6, ENABLE);                              //使能USART1，串口开始运行

    uart_init(UART_6, 115200, UART6_TX_C0, UART6_RX_C1);
    uart_interrupt_init(UART_6,ENABLE,UARTINT_RX);

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       无线转串口数据接收标志位查询
// @param       void
// @return      uint8:              1代表收到数据，0代表未收到数据，标志依靠中断更新
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
uint8 wireless_uart_get_receive_flag(){
    uint8 temp;
    temp = receiveSuccessFlag;
    receiveSuccessFlag = 0;
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取接收到的数据
// @param       void
// @return      char:               接收到的字符
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
char wireless_uart_get_received_data(){
    return receiveDat;
}
