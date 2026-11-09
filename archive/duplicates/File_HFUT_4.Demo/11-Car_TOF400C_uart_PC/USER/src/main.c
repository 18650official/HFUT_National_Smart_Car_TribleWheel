/*********************************************************************************************************************
* @file            main.c
* @brief           TOF400C测距模块例程
* @author          lemon
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2024-4-30
*
********************************************************************************************************************/

/* 用户头文件 */
#include "main.h"
#include "headfile.h"

/** ！！！！！！！！
* tof400c测距模块相关文件在该项目下的 -> hefei_peripheral -> dmx_tof400c.c和dmx_tof400c.h中
* 该例程为TOF400C测距模块的距离采集例程,将测距距离发送到串口助手
！！！！！！！！**/

unsigned int get_dis;

/* 主函数 */
int main(void)
{
    /* 此处声明需要用到的局部变量 */

    /* 智能车初始化 */
    car_init();

    /* 此处编写单次运行的代码(例如：初始化代码等) */

    // 初始化TOF400C测距模块
    Init_TOF400C();
    // 发送字符串
    printf("This is DMX TOF400C example!\r\n");

    while(1)
    {
        /* 此处编写需要循环执行的代码 */

        // 获取TOF400C模块测量距离
        get_dis = Get_Distance_TOF400C();
        // 发送到串口助手
        printf("distance:%d mm\r\n",get_dis);
        // 延时5ms
        Delay_Ms(1000);

    }
}





