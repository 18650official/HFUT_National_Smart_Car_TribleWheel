
/*********************************************************************************************************************
* @file            HF_HC_SR04.c
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-9-19
 ********************************************************************************************************************/

#include "HF_HC_SR04.h"

#define max     5000U
#define prescal 72U
#define speed   334.52

unsigned long g_overflow_times;
unsigned int g_current_time_value;
unsigned char g_flag_unhandler;
unsigned char g_error_flag = 0;

//-------------------------------------------------------------------------------------------------------------------
// @brief        超声波初始化
// @param        void
// @return       void
// Sample usage:        ultrasonic_init();
                        //初始化超声波功能
//-------------------------------------------------------------------------------------------------------------------
void ultra_init(void)
{
    /* TRIG引脚初始化 */
    gpio_init(TRIG, GPO, 0, SPEED_50MHZ | OUT_PP);                //PA0,初始化为GPIO功能、输出模式、输出高电平、速度50MHZ 推挽输出

    /* ECHO引脚初始化 */
    gpio_init(ECHO, GPI, 0, IN_FLOAT);
    gpio_bit_output(ECHO, 0);                                   //PA4 下拉

    timbase_init(TIM_4, max, prescal);
    TIM_Cmd(TIM4, DISABLE);

    g_overflow_times = 0;
    g_current_time_value = 0;
    g_flag_unhandler = 0;

}

//-------------------------------------------------------------------------------------------------------------------
// @brief        超声波发射一次并返回距离
// @param        void
// @return       unsigned int       测得的距离
// Sample usage:        ultrasonic_output();
                        //获取超声波测得的距离
//-------------------------------------------------------------------------------------------------------------------
unsigned int ultra_get_distance(void)
{
    unsigned int distance;
    int i;

    distance = 0;
    //检测传感器是否出错
    if(g_error_flag == 1){
        return distance;
    }

    while (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == 1);
    /* 超声波trig引脚置1 */
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
    Delay_Us(30);
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    i = 0;

    while(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == 0){
        i++;
        if(i > 30000){
            g_error_flag = 1;
            i = 0;
            return 0; //长时间未响应，标记为错误，强制退出
        }
    }
    i = 0;
    TIM_SetCounter(TIM4, 0);// reconfig timer
    TIM_Cmd(TIM4, ENABLE);
    g_current_time_value = 0;
    g_overflow_times = 0;
    while(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == 1);
    TIM_Cmd(TIM4, DISABLE);
    g_current_time_value = TIM_GetCounter(TIM4);// get interval time

//    while (g_flag_unhandler)
//    {
        g_flag_unhandler = 0;
        /* 计算单次求得的距离 */
        distance = (unsigned int)100 * ((g_overflow_times + (float)g_current_time_value / max) * max * prescal / (float)72000000 * speed * 100) / 2;
//    }

    g_overflow_times = 0;
    g_current_time_value = 0;

    return distance;
}

unsigned int distance_filter(int inValue){
    static int Queue[3];
    int full_state = 1;
    float temp_result;
    unsigned int result;

    //新元素入队列
    if(inValue >= 150 && inValue <= 7000){
        Queue[0] = Queue[1];
        Queue[1] = Queue[2];
        Queue[2] = inValue;
    }

    if(Queue[0] == 0 && Queue[1] == 0) full_state = 1;
    else if(Queue[0] == 0) full_state = 2;
    else full_state = 3;

    switch(full_state){
    case 1:{
        temp_result = Queue[2];
        break;
    }
    case 2:{
        temp_result = 0.75*Queue[2] + 0.25*Queue[1];
        break;
    }
    case 3:{
        temp_result = 0.70*Queue[2]+0.20*Queue[1]+0.10*Queue[0];
        break;
    }
    default : break;
    }
    result = (int)temp_result;
    return result;
}

unsigned int ultra_get_average_distance(){
    return distance_filter(ultra_get_distance());
}


/*
 *==============================================================================
 *函数名称：ultra_init
 *函数功能：初始化HC-SR04
 *输入参数：无
 *返回值：无
 *备  注：初始化HC-SR04引脚的同时，初始化了TIM2，用来记录高电平持续时间
          初始化完TIM2后，没有使能，当Echo收到高电平后使能
 *==============================================================================
 */
//void ultra_init (void)   // Hc-sr04初始化
//{
//    // 结构体定义
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   // 生成用于定时器设置的结构体
//    GPIO_InitTypeDef GPIO_InitStructure;   // GPIO结构体
//    NVIC_InitTypeDef NVIC_InitStructure;   // NVIC结构体
//    EXTI_InitTypeDef EXTI_InitStructure;                        //定义结构体变量
//
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // 使能GPIO时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //开启AFIO的时钟，外部中断必须开启AFIO的时钟
//
//    // GPIO初始化
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;   // 发送电平引脚
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽式输出
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   // 返回电平引脚
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 浮空输入
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOD,GPIO_Pin_9);
//
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource9);
//
//    EXTI_InitStructure.EXTI_Line = EXTI_Line9;                 //选择配置外部中断的14号线
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //指定外部中断线使能
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //指定外部中断线为中断模式
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;     //指定外部中断线为下降沿触发
//    EXTI_Init(&EXTI_InitStructure);                             //将结构体变量交给EXTI_Init，配置EXTI外设
//
//    // 配置定时器基础结构体
//    TIM_DeInit(TIM2);
//    TIM_TimeBaseStructure.TIM_Period = (1000-1);   // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到1000为1ms )
//    TIM_TimeBaseStructure.TIM_Prescaler =(72-1);   // 设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   // 不分频
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // TIM向上计数模式
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);   // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//
//    TIM_ClearFlag(TIM2, TIM_FLAG_Update);   // 清除更新中断，免得一打开中断立即产生中断
//    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   // 打开定时器更新中断
//
//    // NVIC配置
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//
//    NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;   // 选择定时器2中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 抢占式中断优先级设置为0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   // 响应式中断优先级设置为0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能中断
//    NVIC_Init(&NVIC_InitStructure);
//
//    NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;   // 选择定时器2中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 抢占式中断优先级设置为0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   // 响应式中断优先级设置为0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能中断
//    NVIC_Init(&NVIC_InitStructure);
//
//    TIM_Cmd(TIM2,DISABLE);
//    g_overflow_times = 0;
//    g_current_time_value = 0;
//    g_flag_unhandler = 0;
//}

