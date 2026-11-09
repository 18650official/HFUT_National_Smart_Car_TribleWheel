/*********************************************************************************************************************
* @file            main.c
* @brief           智能车超声波测距例程
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-9-19
* TFT模块改变显示颜色
********************************************************************************************************************/


#include "main.h"
#include "headfile.h"

int main(void)
{
    /* 此处声明需要用到的局部变量 */
    unsigned int  distance_value = 0;
    unsigned char distance_ascii[6];
    unsigned long t = 0;

    /* 此处编写单次运行的代码(例如：初始化代码等) */

    /* 智能车初始化 */
    car_init();

    ultra_init();
    nvic_init(TIM2_IRQn, 1, 1, ENABLE);

    ips114_init();
    ips114_set_color(RGB565_WHITE,RGB565_BLACK); //设置为黑底白字显示
    ips114_full(0x000000); //使用黑色填充显示画面
    ips114_show_string(0, 0, "Wave:");


    /* 此处编写单次运行的代码(例如：初始化代码等) */

    while(1)
    {
        /* 此处编写循环运行的代码 */
        ips114_show_int(0, 20, distance_value, 7);
        ips114_show_int(0, 40, t, 5);
        ips114_show_int(0, 60, GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9), 1);
        distance_value = ultra_get_distance();              //超声波模块测距

        //int_to_ascii(distance_value, distance_ascii);       //将测得距离转换为ascii码

        //uart_send_distance_ascii(distance_ascii);           //串口发送测得的距离
        Delay_Ms(500);
        t++;
    }
}

void uart_send_distance_ascii(unsigned char *ascii_value)
{
    uart_send_char(UART_1, ascii_value[1]);
    uart_send_char(UART_1, ascii_value[2]);
    uart_send_char(UART_1, ascii_value[3]);
    uart_send_char(UART_1, '.');
    uart_send_char(UART_1, ascii_value[4]);
    uart_send_char(UART_1, 0x0D);
    uart_send_char(UART_1, 0x0A);                            //换行
}
