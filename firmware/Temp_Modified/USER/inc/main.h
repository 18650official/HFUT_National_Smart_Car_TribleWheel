/*********************************************************************************************************************
* @file            main.h
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-3-2
********************************************************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_

#include "headfile.h"
#include "isr.h"

//特殊名称宏定义
#define SPEED_PARAM_INIT 1.06
#define SCAN_WIDTH 15 //灰度扫描宽
#define IMAGELEN 188
#define IMAGEWID 120
#define YPOS_REFERENCE (IMAGEWID / 2) + 30 //中线坐标定位点
#define ISLAND_L 0x01
#define ISLAND_R 0x02
#define SINGLE_REPAIR_PARAM 20 //单侧补线改变的宽度
#define SPEED_REF 3500
#define OBS_GRAY_GATE_L 90 //障碍物部分的灰度大小-下阈值
#define OBS_GRAY_GATE_H 150 //障碍物部分的灰度大小-上阈值

typedef struct{ //存放赛道定位线的结构体
    unsigned char Left_line_array[IMAGEWID];
    unsigned char Middle_line_array[IMAGEWID];
    unsigned char Right_line_array[IMAGEWID];
}Linedata_Typedef;

typedef enum{
    RACE_NO_START,       //比赛未开始
    RACE_OUT_PARKINGLOT, //出库
    RACE_DEFAULT_RUN,    //正常通行
    RACE_FINISH_CAR_A,   //A车比赛完成标志
}RaceStage_Typedef;

typedef struct{
    uint8_t Point_C[2]; //近处路口左边界
    uint8_t Point_D[2]; //近处路口右边界
    uint8_t Point_E[2]; //远处路口左边界
    uint8_t Point_F[2]; //远处路口右边界
    uint8_t Point_Average[2];
    uint8_t Point_flag[6]; //A-F六点的判定标志
}CrossingPosition_Typedef; //十字路口的四点定位

//扩展函数声明
//图像综合处理函数
void ImageBinarization(uint8_t OriImage[IMAGEWID][IMAGELEN],uint8_t OutImage[IMAGEWID][IMAGELEN],int threshold);
uint8 getThreshold(uint8 image[IMAGEWID][IMAGELEN],uint16 len,uint16 wid);
char processImage(void);
int Threshold_filter(int inValue);
void LinesGenerate(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata);
void getRoadError_Default(Linedata_Typedef* linedata,int Ypositon,int MiddleRef,int* MiddleXDataPtr,int* MiddleErrorPtr);
//车辆运动控制函数
void motor_motion(int16_t speed_L,int16_t speed_R,float speed_param);
void Car_run_with_error(int basic_speed,int error,float error_param,float speed_param);
void Car_left_out();
void AvoidObs();
void quarterTurn();
void bridgeAdjust(void);
//特征识别函数及内部方法
float calculateMissLineRatio(char direct_select,Linedata_Typedef* linedata,uint8_t yPos_start,uint8_t yPos_end);
int calculateMissLine(char direct_select,Linedata_Typedef* linedata,uint8_t yPos_start,uint8_t yPos_end);
char isLineMissed(char direct_select,Linedata_Typedef* linedata,uint8_t yPos);
uint8_t crossing_judge_down_point(Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos,
uint8_t,uint8_t);
uint8_t crossing_judge_up_point(Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos,
uint8_t,uint8_t);
void isCrossing(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos);
uint8_t L_to_R_scan(uint8_t OutImage[IMAGEWID][IMAGELEN],uint8_t xPos_L,uint8_t xPos_R,uint8*,uint8*);
void isZebra(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata);
void isObstruct(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata);
void is90Dg(uint8 OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata);
void isSingleBridge(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata);
//补线函数
void Repair_and_RefreshData(uint8_t OutImage[IMAGEWID][IMAGELEN],CrossingPosition_Typedef* crossing_pos,Linedata_Typedef* linedata);
void reGenerate_MidLine(Linedata_Typedef* linedata);
void editLineMap_K(Linedata_Typedef* linedata,char direct,uint8_t yPos_start,uint8_t yPos_end,float k);
void editLineMap_connect(Linedata_Typedef* linedata,char direct_select,uint8_t x1,uint8_t x2,uint8_t y1,uint8_t y2);
float getAverageK(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3);
//其他
void Draw_Locationline(Linedata_Typedef* linedata);
int16_t Limit_int(int16_t dat,int16_t min,int16_t max);
void displayInfo(uint8_t page);
void clearFlag();


//全局变量表
uint8_t key_value; //Key ReadValue
uint8_t key_value_to_process = 0x00; //待处理状态值
int8_t key_flag_1 = 0;
int8_t key_flag2 = 0;
float speed_param = SPEED_PARAM_INIT; //速度比例,本车在速度4000时为1.06平衡
float run_speed_param_compensate = 0.008; //转弯速度补偿系数
float run_error_param = 1.20; //误差系数
int midLineAxis; //中线的平均横坐标
int midLineError; //中线偏移
unsigned int Wave_distance; //超声波测距的距离

uint8_t OutImage[IMAGEWID][IMAGELEN]; //输出图像
uint8_t Up_down_boundary[IMAGELEN]; //左右扫描对应的纵坐标集合
uint8_t Left_right_boundary[IMAGEWID]; //左右两侧的突变边界
uint16_t GrayScale_count[256] = {0}; //灰度计数
uint8_t MiddleLinePos_Buffer[5];
char isImageProcessed;
char dispMode = 0;
char race_flag = 0; //比赛开始标志位
char line_disp_flag = 1; //赛道定位线显示
char crossing_flag = 0;  //十字路口判断
char crossing_stage = 0; //判断十字路口所处阶段
char zebra_flag = 0; //斑马线
char obstruct_flag = 0; //障碍物的标志位
char is90dg_flag = 0; //90度转弯标志位
char bridge_flag = 0; //单边桥识别标志位
char bridge_stage = 0; //通过单边桥的阶段

//需要显示的调试数据
unsigned char Dual_missed_count;
uint8 tz_down = 0; //扫描中线向左侧的突变点
uint8 tz_up = 0; //扫描中线向右侧的突变点

Linedata_Typedef linedata;
RaceStage_Typedef race_Stage; //比赛阶段枚举
CrossingPosition_Typedef crossing_pos;

#endif /* MAIN_H_ */
