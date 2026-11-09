/*********************************************************************************************************************
* @file            car_init.c
* @author          Andreas HF
* @Target core     CH32V307VCT6
* @revisions       2022.09.07, V1.0
* @modify          none
********************************************************************************************************************/

#include "car_init.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief        左右轮pwm初始化
// @param        void
// @return       void
// Sample usage:        car_init();
                        //智能车初始化
//-------------------------------------------------------------------------------------------------------------------

//定义引脚
//#define CH9143_UART UART_2
//#define CH9143_UART_BAUD 115200
//#define CH9143_UART_TX_PIN UART2_TX_A2
//#define CH9143_UART_RX_PIN UART2_RX_A3

void car_init()
{
//    DisableGlobalIRQ();                                         //默认关所有中断
    interrupt_global_disable();
    board_init();
    Delay_Init();

    /* 初始化电机 */
    motor_init(left);
    motor_init(right);

    //初始化超声波测距传感器
    ultra_init();

    //初始化屏幕
    ips114_init();     //初始化屏幕
    ips114_set_color(RGB565_WHITE,RGB565_BLACK); //设置为黑底白字显示
    ips114_full(0x000000); //使用黑色填充显示画面

    /* 初始化按键 */
    key_init(K1);
    key_init(K2);
    key_init(K3);
    key_init(K4);
    key_init(K5);
    key_init(K6);

    adc_init(ADC_IN2_A2);
    adc_init(ADC_IN3_A3);
    adc_init(ADC_IN4_A4);
    adc_init(ADC_IN5_A5);
    adc_init(ADC_IN6_A6);

    /* 初始化板载LED */
    board_led_init(LED1);
    board_led_init(LED2);

    /* 初始化RGB引脚 */
    ws2812b_init();

    // 初始化CH9143蓝牙模块
    wireless_uart_init();
    //uart_init(CH9143_UART,CH9143_UART_BAUD,CH9143_UART_TX_PIN,CH9143_UART_RX_PIN);

//    /* 初始化干簧管 */
//    reed_init();

    /* 初始化中断 */
    int_init();
    interrupt_global_enable();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        中断初始化
// @param        void
// @return       void
// Sample usage:        int_init();//中断初始化
//-------------------------------------------------------------------------------------------------------------------
void int_init(void)
{
    //Attach Interrupt
    exti_enable(EXTI_Line3, C3, BOTH);
    exti_enable(EXTI_Line4, C4, BOTH);
    exti_enable(EXTI_Line5, C5, BOTH);
    nvic_init(TIM4_IRQn, 1, 1, ENABLE);
}



