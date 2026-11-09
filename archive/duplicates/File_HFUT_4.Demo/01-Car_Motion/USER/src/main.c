/*********************************************************************************************************************
* @file            main.c
* @brief           智能车行进控制例程
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-9-12
* 小车运动例程
********************************************************************************************************************/

/* 用户头文件 */
#include "main.h"
#include "headfile.h"

void motor_move(int,int);

/* 全局变量声明 */


/* 主函数 */
int main()
{
    /* 此处声明需要用到的局部变量 */
    /* 智能车初始化 */
    car_init();
    motor_move(0,0);
    Delay_Ms(1000);

    //1
    motor_move(4000,2200);
    Delay_Ms(400);
    motor_move(4000,4000);
    Delay_Ms(400);
    motor_move(1300,4000);
    Delay_Ms(750);
    motor_move(4000,4000);
    Delay_Ms(550);


    motor_move(0,0); //stop


    /* 此处编写单次运行的代码(例如：初始化代码等) */


    while(1)
    {

        /* 此处编写需要循环执行的代码 */

    }
}

void motor_move(int l,int r){
    motor_forward(left, l);
    motor_forward(right,r);
}
