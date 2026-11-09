/*********************************************************************************************************************
* @file            main.c
* @brief           智能车串口中断例程
* @author          GX
* @version         1.0
* @Target core     CH32V103R8T6
* @date            -2021.10.22, V1.0
* 打开串口调试助手后，串口调试助手显示消息“a” ,随后主板等待串口接收数据，将接收到的数据通过串口发送。
********************************************************************************************************************/

/* 用户头文件 */
#include "main.h"

/* 全局变量声明 */


/* 主函数 */
int main(void)
{
   /* 此处声明需要用到的局部变量 */

    /* 智能车初始化 */
    car_init();

    board_led_off(LED1);
    board_led_off(LED2);

    uart_init(UART_2, 115200, UART2_TX_A2, UART2_RX_A3);
    uart_interrupt_init(UART_2,ENABLE,UARTINT_RX);  //打开无线串口的接收中断

    /* 此处编写单次运行的代码(例如：初始化代码等) */
    g_uart_flag = 0;                                  //设定标志位为0
    uart_send_char(UART_2, 'a');                      //串口1发送字符

    while(1)
    {
        board_led_on(LED1);
        /* 此处编写循环运行的代码 */
        if(g_uart_flag == 1)        //当检测到标志位为1时，串口3发送接收到的数据
        {
            board_led_on(LED2);
            Delay_Ms(10);
            g_uart_flag = 0;
            uart_send_char(UART_2, g_rx_char);
        }
        board_led_off(LED2);
    }
}
