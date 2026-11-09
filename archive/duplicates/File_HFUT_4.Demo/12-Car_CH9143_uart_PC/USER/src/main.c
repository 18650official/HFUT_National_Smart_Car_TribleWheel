/*********************************************************************************************************************
* @file            main.c
* @brief           CH9143蓝牙模块收发测试例程
* @author          lemon
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2024-4-30
*
********************************************************************************************************************/

/** ！！！！！！！！
* ch9143蓝牙模块采用串口通讯
* 相关文件在该项目下的 -> hefei_libraries -> hf_uart.c和hf_uart.h中
* 该例程为两个CH9143蓝牙模块通信例程
* 电脑端通过USB~TTL连接到CH9143模块
* 开发板端将CH9143模块插入主板预留的无线接口的位置
* 电脑端通过串口助手发送字符到主板,主板给电脑端返回接收到的数据
* 如：电脑端通过串口助手发送"DMX",随后收到回复"DMX"
* 注：本模块具有详细的说明文档,可在资料中查看
！！！！！！！！**/

/* 用户头文件 */
#include "main.h"
#include "headfile.h"

// CH9143蓝牙模块串口号
#define CH9143_UART             UART_2
// CH9143蓝牙模块波特率
#define CH9143_UART_BAUD        115200
// CH9143蓝牙模块TXD引脚
#define CH9143_UART_TX_PIN      UART2_TX_A2
// CH9143蓝牙模块RXD引脚
#define CH9143_UART_RX_PIN      UART2_RX_A3

/* 主函数 */
int main(void)
{
    /* 此处声明需要用到的局部变量 */

    /* 智能车初始化 */
    car_init();

    /* 此处编写单次运行的代码(例如：初始化代码等) */

    // 初始化CH9143蓝牙模块
    uart_init(CH9143_UART,CH9143_UART_BAUD,CH9143_UART_TX_PIN,CH9143_UART_RX_PIN);
    // 开启串口接收中断
    uart_interrupt_init(CH9143_UART,ENABLE,UARTINT_RX);
    // 发送字符串
    uart_putstr(CH9143_UART,"This is DMX CH9143 example!\r\n");

    while(1)
    {
        /* 此处编写需要循环执行的代码 */

        // 该例程为UART收发测试,UART中断相关函数在该项目下的 -> user_c -> isr.c文件中

    }
}





