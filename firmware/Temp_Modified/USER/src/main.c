/* 用户头文件 */
#include "car_init.h"
#include "board.h"
#include "main.h"
#include "headfile.h"

/*
 * 任务说明
 * ①处理图像，识别道路边界和中线，沿道路行驶
 * ②出入库 - 完成
 * ③通过单边桥
 * ④实现S型避障
 * ⑤使用TOF测量隧道长度 - 已完成
 * ⑥使用无线电进行双车通信和正确出入库 - 已完成
 * ⑦识别斑马线、及时停车 - 已完成
*/

/*
 * 按键顺序
 *   1     5
 * 2   4
 *   3     6
*/

void setup(){ //初始化函数
    car_init();  //汽车启动

    ips114_show_string(0,0,"CAM-IPS");
    ips114_show_string(0,20,"Snowmiku-V2.0");

    mt9v03x_init_dvp(); //Init the camera
    Delay_Ms(500);
    ips114_clear();
}

void loop(){  //循环执行
    //静态变量声明
    static char page = 0;
    static unsigned char wave_count = 0;
    uint8 wireless_received_flag = 0;
    //读取按键
    key_value = key_check();
    if(key_value != 0xFF){
        key_value_to_process = key_value;
    }
    //判定按键执行操作
    if(key_value_to_process){
        switch(key_value_to_process){
        case 0x06:{
            key_value_to_process = 0x00;
            dispMode = 0;
            break;
        }
        case 0x05:{
            key_value_to_process = 0x00;
            dispMode = 1;
            break;
                }
        case 0x04:{
            key_value_to_process = 0x00;
            page++;
            if(page>3) page = 3;
            else ips114_clear(); //刷新屏幕
            break;
        }
        case 0x03:{
            key_value_to_process = 0x00;
            line_disp_flag = (line_disp_flag == 0 ? 1 : 0); //切换判定线显示
            break;
        }
        case 0x02:{
            key_value_to_process = 0x00;
            page--;
            if(page<0) page = 0;
            else ips114_clear(); //刷新屏幕
            break;
        }
        case 0x01:{
            key_value_to_process = 0x00;
            if(!race_flag){
                clearFlag();
                race_flag = 1;
            }
            else{
                race_flag = 0;
                motor_motion(0, 0, speed_param); //手动控制汽车停下
            }
            break;
         }

        default:
            break;
        }
    }

    //更新显示数据
    displayInfo(page);
    //更新摄像头的图像

    if(mt9v03x_finish_flag_dvp){
        mt9v03x_finish_flag_dvp = 0;
        isImageProcessed = processImage();
    }

    if(isImageProcessed == 1){
        //刷新一帧
        isImageProcessed = 0;
        if(dispMode == 0) ips114_show_gray_image(0, 20, OutImage[0], IMAGELEN, IMAGEWID, IMAGELEN, IMAGEWID, 0);
        else if(dispMode == 1) ips114_show_gray_image(0,20,mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H,MT9V03X_DVP_W, MT9V03X_DVP_H,0);
    }

    //控制车辆行驶
    if(race_flag){
        Car_run_with_error(SPEED_REF, midLineError,run_error_param,run_speed_param_compensate); //小车循迹行驶
     }
    else{
        //检测前车串口的数据
        wireless_received_flag = wireless_uart_get_receive_flag();
        if(wireless_received_flag == 1){
            // 判断接收到的数据
            if(wireless_uart_get_received_data() == 'r'){
                uart_putchar(UART_6, 'r');
                //此处填写出库操作
                Car_left_out(); //左转出库
                uart_putchar(UART_6, 's'); //成功出库，返回数据
                race_flag = 1;
            }
        }
    }

    //更新计数,间隔一段时间刷新距离传感器
    if(obstruct_flag != 2){
        wave_count++;
        if(wave_count == 5 && obstruct_flag != 2){
            wave_count = 0;
            Wave_distance = ultra_get_average_distance(); //获取障碍物距离（加权平均数）
        }
    }
    else{
        //关闭超声波传感器
        wave_count = 0;
        Wave_distance = 1;
    }
}

void main(){
    setup();
    while(1){
        loop();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       显示屏数据刷新
// @param       page:           指定的页数
// @return      void
// Sample usage:                实时i刷新车辆的各种参数
//-------------------------------------------------------------------------------------------------------------------

void displayInfo(uint8_t page){
    ips114_show_string(0,0,"CAMERA:");
    ips114_show_string(135, 5, "See:");
    if(crossing_flag) ips114_show_char(175, 5, 'C');
    else if(zebra_flag) ips114_show_char(175, 5, 'Z');
    else if(obstruct_flag == 1 ) ips114_show_char(175, 5, 'O');
    else if(is90dg_flag) ips114_show_char(175, 5, 'L');
    else ips114_show_char(175, 5, ' ');
    ips114_show_string(188, 5, "T:"); //显示二值化阈值
    ips114_show_int(200, 5, Threshold_filter(getThreshold(mt9v03x_image_dvp, IMAGELEN, IMAGEWID)), 4);
    //按照页数显示图像信息
    if(page == 1){
        ips114_show_string(192,20, "Lm:");
        ips114_show_int(192, 35, calculateMissLine(0x01, &linedata, IMAGEWID-1, 0), 3);
        ips114_show_string(192,50, "Rm:");
        ips114_show_int(192, 65, calculateMissLine(0x02, &linedata, IMAGEWID-1, 0), 3);
        ips114_show_string(192,80, "Dm:");
        ips114_show_int(192, 95, calculateMissLine(0x03, &linedata, IMAGEWID-1, 0), 3);
    }
    else if(page == 0){
        ips114_show_string(192,20, "M:");
        ips114_show_int(192, 35, midLineAxis, 3);
        ips114_show_string(192,50, "Err:");
        ips114_show_int(192, 65, midLineError, 3);
    }
    else if(page == 2){
        ips114_show_string(192,20, "Cs:");
        ips114_show_int(192, 35, crossing_stage, 3);
        ips114_show_string(192,50, "Dis:");
        if(Wave_distance > 1) ips114_show_int(192, 65, Wave_distance, 5); //显示超声波距离
        else if(Wave_distance == 1) ips114_show_string(192, 65, "FIN "); //表示完成避障
        else if(Wave_distance == 0) ips114_show_string(192, 65, "ERR "); //表示传感器出错
        ips114_show_string(192,80, "Obs:");
        ips114_show_int(192, 95, obstruct_flag, 1);
    }
    else if(page == 3){
        ips114_show_string(192,20, "Bf:");
        ips114_show_int(192, 35, bridge_flag, 1);
        ips114_show_string(192,50, "Bs:");
        if(Wave_distance > 1) ips114_show_int(192, 65, bridge_stage, 1);
        ips114_show_string(192,80, "U/D:");
        ips114_show_int(192, 95, tz_up, 1);
        ips114_show_int(202, 95, tz_down, 1);
    }
    //显示行车状态
    if(race_flag) ips114_show_string(192, 110, "RUN ");
    else ips114_show_string(192, 110, "STOP");

    //显示道路判定线
    if(line_disp_flag == 1){
        Draw_Locationline(&linedata);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       优化后的汽车动作
// @param       speed_L:        An integar below 7000.
//              speed_R:        An integar below 7000.
//              speed_param:    右车轮与左车轮的速度之比
// @return      void
// Sample usage:                控制两轮动作不均衡的车辆平稳行驶
//-------------------------------------------------------------------------------------------------------------------
void motor_motion(int16_t speed_L,int16_t speed_R,float speed_param){
    float left_val,right_val;
    left_val = speed_L;
    right_val = (float)speed_R * speed_param;

    motor_forward(left, (int)left_val);
    motor_forward(right, (int)right_val);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       含有误差的车辆前进操作
// @param       basic_speed:     基础速度
//              error:           摄像头捕捉的误差大小
//              error_param:     误差前的系数
//              speed_param:     速度前的补偿系数
// @return      void
// Sample usage:                图像二值化
//-------------------------------------------------------------------------------------------------------------------

void Car_run_with_error(int basic_speed,int error,float error_param,float speed_param){
    float add_value; //叠加在车轮的增益
    add_value = error * (basic_speed*speed_param+error_param); //计算最终的输出量
    motor_motion(basic_speed+(int)add_value,basic_speed-(int)add_value,SPEED_PARAM_INIT);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       车辆向左转弯出库
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Car_left_out()
{
    motor_motion(3200, 3200, SPEED_PARAM_INIT); //直行一段距
    Delay_Ms(350);
    motor_motion(2400, 3800, SPEED_PARAM_INIT); //向左打角
    Delay_Ms(1100);
    motor_motion(SPEED_REF, SPEED_REF, SPEED_PARAM_INIT); //最终状态改为直行
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       车辆躲避障碍
// @param       void
// @return      void
// Sample usage:    避障的动作函数
//-------------------------------------------------------------------------------------------------------------------

void AvoidObs()
{
    //1
    motor_motion(4000,1700,1);
    Delay_Ms(400);
    motor_motion(4000,4000,1);
    Delay_Ms(290);
    motor_motion(3400,3400,1);
    Delay_Ms(70);
    motor_motion(1400,4000,1);
    Delay_Ms(785);
    motor_motion(4000,4000,1);
    Delay_Ms(775);
    motor_motion(4000,2200,1);
    Delay_Ms(260);
    motor_motion(4000,4000,1);
    Delay_Ms(370);
    motor_motion(4000,1000,1);
    Delay_Ms(500);
    motor_motion(4000,4000,1);
    Delay_Ms(370);
    motor_motion(1000,4000,1);
    Delay_Ms(300);

    motor_motion(SPEED_REF, SPEED_REF, SPEED_PARAM_INIT); //最终状态改为直行
    obstruct_flag = 2; //代表执行完成，距离传感器不再进行扫描
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      快速大津
//  @return     uint8
//  @since      v1.1
//  Sample usage:   OTSU_Threshold = otsuThreshold(mt9v03x_image_dvp[0]);//大津法阈值
//-------------------------------------------------------------------------------------------------------------------
uint8 getThreshold(uint8 image[IMAGEWID][IMAGELEN],uint16 len,uint16 wid)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
    static uint8 last_threshold; //上一次的阈值，用于避免突发抖动
    int Pixel_Max=0;
    int Pixel_Min=255;
    uint16 width = len;
    uint16 height = wid;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = (uint8*)image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
            if(data[i * width + j]>Pixel_Max)   Pixel_Max=data[i * width + j];
            if(data[i * width + j]<Pixel_Min)   Pixel_Min=data[i * width + j];
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = Pixel_Min; i < Pixel_Max; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = Pixel_Min; j < Pixel_Max; j++)
    {

        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = (float)(w0 *w1* (u0 - u1)* (u0 - u1)) ;
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }

    }

    if(threshold>90 && threshold<130)
        last_threshold = threshold;
    else
        threshold = last_threshold;

    return threshold;
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       图像二值化
// @param       OriImage:       摄像头捕获的原始图像
//              OutImage:       输出的二维数组
//              threshold:      阈值
// @return      void
// Sample usage:                图像二值化
//-------------------------------------------------------------------------------------------------------------------
void ImageBinarization(uint8_t OriImage[IMAGEWID][IMAGELEN],uint8_t OutImage[IMAGEWID][IMAGELEN],int threshold){
    int i;
    for(i=0;i<IMAGEWID*IMAGELEN;i++){
        if(OriImage[i/IMAGELEN][i%IMAGELEN] >= threshold){
            OutImage[i/IMAGELEN][i%IMAGELEN] = 0xFF;
        }
        else{
            OutImage[i/IMAGELEN][i%IMAGELEN] = 0x00;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       二值化滤波器
// @param       inValue:       提取的图片灰度阈值
// @return      int：                              平均的图像阈值
// Sample usage:               二值化的滤波，保证显示不会产生频闪，影响行驶
//-------------------------------------------------------------------------------------------------------------------
int Threshold_filter(int inValue){
    static int Queue[3];
    int full_state = 1;
    float temp_result;
    int result;

    //新元素入队列
    if(inValue >= 50 && inValue <= 150){
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

//-------------------------------------------------------------------------------------------------------------------
// @brief       边界线定位函数
// @param       OutImage:    经过二值化处理的图像
//              Linedata_Typedef* linedata: 存放定位线数据的结构体指针
// @return      void
// Sample usage:                图像二值化
//-------------------------------------------------------------------------------------------------------------------

void LinesGenerate(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata){
    int i;
    int currentRow; //当前所在扫描行

    //初始化，找出最下方的三个定位点
    currentRow = IMAGEWID - 1;
    linedata->Middle_line_array[currentRow] = IMAGELEN/2; //给定中线的预设值

    for(i=linedata->Middle_line_array[currentRow];i>0;i--)//从中间向左找上升沿
    {
        if(OutImage[currentRow][i]==0xFF && OutImage[currentRow][i-1]==0x00){
            //判断到左侧边界的下降点
            linedata->Left_line_array[currentRow] = i-1;
        }
        else if(i==1 && OutImage[currentRow][i-1]==0xFF){
            linedata->Left_line_array[currentRow] = 0;
            break;
        }
    }

    for(i=linedata->Middle_line_array[currentRow];i<IMAGELEN-1;i++)//从中间向右找上升沿
    {
        if(OutImage[currentRow][i]==0xFF && OutImage[currentRow][i+1]==0x00){
            //判断到左侧边界的下降点
            linedata->Right_line_array[currentRow] = i+1;
            break;
        }
        else if(i==IMAGELEN-2 && OutImage[currentRow][i+1]==0xFF){
            linedata->Right_line_array[currentRow] = i+1;
            break;
        }
    }
    linedata->Middle_line_array[currentRow] = (linedata->Left_line_array[currentRow]+linedata->Right_line_array[currentRow])/2;
    //中线为左右的平均数

    currentRow--;
    for(;currentRow>0;currentRow--)//向上迭代求出剩下的中心点
    {
        for(i=linedata->Middle_line_array[currentRow];i>0;i--)//从中间向左找上升沿
        {
            if(OutImage[currentRow][i]==0xFF && OutImage[currentRow][i-1]==0x00){
                //判断到左侧边界的下降点
                linedata->Left_line_array[currentRow] = i-1;
            }
            else if(i==1 && OutImage[currentRow][i-1]==0xFF){
                linedata->Left_line_array[currentRow] = 0;
                break;
            }
        }

        for(i=linedata->Middle_line_array[currentRow];i<IMAGELEN-1;i++)//从中间向右找上升沿
        {
            if(OutImage[currentRow][i]==0xFF && OutImage[currentRow][i+1]==0x00){
                //判断到左侧边界的下降点
                linedata->Right_line_array[currentRow] = i+1;
                break;
            }
            else if(i==IMAGELEN-2 && OutImage[currentRow][i+1]==0xFF){
                linedata->Right_line_array[currentRow] = i+1;
                break;
            }
        }
        linedata->Middle_line_array[currentRow] = (linedata->Left_line_array[currentRow]+linedata->Right_line_array[currentRow])/2;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       赛道检测线绘制函数
// @param       Linedata_Typedef* linedata； 存放定位线数据的结构体指针
// @return      void
// Sample usage:                在显示器上显示道路边界线
//-------------------------------------------------------------------------------------------------------------------

void Draw_Locationline(Linedata_Typedef* linedata){
    int i;
    for(i=IMAGEWID;i>0;i--){
        if(linedata->Left_line_array[i]<=IMAGELEN &&
           linedata->Right_line_array[i]<=IMAGELEN){
            ips114_draw_point(linedata->Left_line_array[i],20+i, RGB565_BROWN);
            ips114_draw_point(linedata->Middle_line_array[i],20+i, RGB565_BLUE);
            ips114_draw_point(linedata->Right_line_array[i],20+i, RGB565_BROWN);
            ips114_draw_point(linedata->Left_line_array[i]+1,20+i, RGB565_BROWN);
            ips114_draw_point(linedata->Middle_line_array[i]+1,20+i, RGB565_BLUE);
            ips114_draw_point(linedata->Right_line_array[i]+1,20+i, RGB565_BROWN);
            ips114_draw_point(linedata->Left_line_array[i]-1,20+i, RGB565_BROWN);
            ips114_draw_point(linedata->Middle_line_array[i]-1,20+i, RGB565_BLUE);
            ips114_draw_point(linedata->Right_line_array[i]-1,20+i, RGB565_BROWN);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中线数据解算函数
// @param       dat；                                                                   原始数据
//              min；                                                                   最小值
//              max：                                                                   最大值
// @return      int：                                                                   限制幅度后的数据
// Sample usage:                            限制传入原始数据的幅度
//-------------------------------------------------------------------------------------------------------------------
int16_t Limit_int(int16_t dat,int16_t min,int16_t max){
    if(dat<min) return min;
    else if(dat>max) return max;
    else return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中线数据解算函数
// @param       Linedata_Typedef* linedata； 存放定位线数据的结构体指针
//              Ypositon；                                                    采样起始的纵坐标，默认为YPOS_REFERENCE
//              MiddleRef：                                                  中线定点的横坐标
//              MiddleXDataPtr:             接收中线坐标的指针
//              MiddleErrorPtr：                                    接收误差的整数指针
// @return      void
// Sample usage:                量化中线误差，为小车运动提供控制参量
//-------------------------------------------------------------------------------------------------------------------

void getRoadError_Default(Linedata_Typedef* linedata,int Ypositon,int MiddleRef,int* MiddleXDataPtr,int* MiddleErrorPtr){
    int Y_Up,i;
    int Middle_XPos_Average;
    Y_Up = Ypositon - 5;

    for(i = 0;i < 5;i++){
        MiddleLinePos_Buffer[i] = linedata->Middle_line_array[Y_Up+i]; //求平均值
    }
    for(i = 0,Middle_XPos_Average = 0;i < 5;i++){
        Middle_XPos_Average += MiddleLinePos_Buffer[i];
    }
    Middle_XPos_Average /= 5;
    *MiddleXDataPtr = Middle_XPos_Average; //传出中线横坐标
    *MiddleErrorPtr = Middle_XPos_Average - MiddleRef; //传出中线的偏移误差
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       图像处理函数
// @param       void
// @return      char state:  1为处理完成，0为未完成刷新
// Sample usage:                图像二值化
//-------------------------------------------------------------------------------------------------------------------
char processImage(void){
    char i;
    ImageBinarization(mt9v03x_image_dvp, OutImage, Threshold_filter(getThreshold(mt9v03x_image_dvp, IMAGELEN, IMAGEWID)));
    LinesGenerate(OutImage, &linedata); //将道路识别线写入结构体
    getRoadError_Default(&linedata, YPOS_REFERENCE, IMAGELEN/2, &midLineAxis, &midLineError); //生成边界线

    //判断其他路口特征
    i = 0;
    while(i<1){ //只要识别到一个特殊区域，立马退出并使用获取的数据补线。防止数据覆盖
        i++;
        isZebra(OutImage, &linedata);
        if(zebra_flag) break;
        is90Dg(OutImage, &linedata);
        if(is90dg_flag){
            if(race_flag == 1) quarterTurn();
            break;
        }
        isSingleBridge(OutImage, &linedata);
        if(bridge_flag){
            if(race_flag == 1) bridgeAdjust();
            break;
        }
        isObstruct(OutImage,&linedata); //判定障碍
        if(obstruct_flag == 1) {
            if(race_flag == 1) AvoidObs(); //在运转状态下，执行躲避动作
            break;
        }
        i++;
    }
    //在存在障碍物数据时，进行补线操作
    if(i==1) Repair_and_RefreshData(OutImage, &crossing_pos,&linedata);
    return 1;
}

//判定线选择宏定义
#define LEFT_BOUNDARY 0x01
#define RIGHT_BOUNDARY 0x02
#define BOTH_BOUNDARY 0x03
//-------------------------------------------------------------------------------------------------------------------
// @brief       判断某个纵坐标是否丢线
// @param       linedata:        标线结构体
//              direct_select:   标线方向，0x01为左，0x02为右
//              yPos:            纵坐标
// @return      char：                                 0为正常，1为丢线
// Sample usage:                 判定点位是否丢线
//-------------------------------------------------------------------------------------------------------------------

char isLineMissed(char direct_select,Linedata_Typedef* linedata,uint8_t yPos){
    if(direct_select == 0x01) //判断左侧丢线
    {
        if(linedata->Left_line_array[yPos] <= 1){
            return 1;
        }
        else return 0;
    }
    else if(direct_select == 0x02){
        if(linedata->Right_line_array[yPos] >= IMAGELEN-2) return 1;
        else return 0;
    }
    else if(direct_select == LEFT_BOUNDARY | RIGHT_BOUNDARY){
        if(linedata->Right_line_array[yPos] >= IMAGELEN-2 && linedata->Left_line_array[yPos] <= 1)
            return 1;
        else return 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       计算丢线数目
// @param       linedata:        标线结构体
//              direct_select:   标线方向，0x01为左，0x02为右
//              yPos_start:      纵坐标起点
//              yPos_end:        纵坐标终点
// @return      int：                                   丢线的数量
// Sample usage:                 计算丢线区间，有助于判断特征
//-------------------------------------------------------------------------------------------------------------------

int calculateMissLine(char direct_select,Linedata_Typedef* linedata,uint8_t yPos_start,uint8_t yPos_end)
{
    int i,count,t;
    if(yPos_start > yPos_end){
        t=yPos_start;
        yPos_start=yPos_end;
        yPos_end=t;
    }
    for(i=yPos_start,count=0;i<=yPos_end;i++){
        if(isLineMissed(direct_select, linedata, i)) count++;
    }
    return count;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       计算丢线区间占比
// @param       linedata:        标线结构体
//              direct_select:   标线方向，0x01为左，0x02为右
//              yPos_start:      纵坐标起点
//              yPos_end:        纵坐标终点
// @return      float：                              丢线的区域在区间的占比
// Sample usage:                 计算丢线区间，有助于判断特征
//-------------------------------------------------------------------------------------------------------------------

float calculateMissLineRatio(char direct_select,Linedata_Typedef* linedata,uint8_t yPos_start,uint8_t yPos_end)
{
    int i,count,t;
    if(yPos_start > yPos_end){
        t=yPos_start;
        yPos_start=yPos_end;
        yPos_end=t;
    }
    for(i=yPos_start,count=0;i<=yPos_end;i++){
        if(isLineMissed(direct_select, linedata, i)) count++;
    }
    return (count/(yPos_end-yPos_start)); //输出丢线占比
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       两点补线
// @param       linedata:        标线结构体
//              direct:          标线方向，0x01为左，0x02为右
//              (x,y):           任意两点坐标
// @return      void
// Sample usage:                补线，适用于拐点较全的情况
//-------------------------------------------------------------------------------------------------------------------

void editLineMap_connect(Linedata_Typedef* linedata,char direct_select,uint8_t x1,uint8_t x2,uint8_t y1,uint8_t y2)
{
    //连接式补线法
    float k,b; //x=ky+b
    unsigned char i;
    k = (x2-x1)/(y2-y1);
    b = x1-(k*y1);
    if(y1<=y2){
        for(i=y1;i<=y2;i++){
            if(direct_select == LEFT_BOUNDARY)
                linedata->Left_line_array[i] = k*i+b;
            else {
                linedata->Right_line_array[i] = k*i+b;
            }
        }
    }
    else{
        for(i=y2;i<=y1;i++){
            if(direct_select == LEFT_BOUNDARY)
                linedata->Left_line_array[i] = k*i+b;
            else {
                linedata->Right_line_array[i] = k*i+b;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       单拐角补线
// @param       linedata:        标线结构体
//              direct:          标线方向，0x01为左，0x02为右
//              yPos_start:      纵坐标起点
//              yPos_end:        纵坐标终点
//              k:               预设（如最小二乘估计）的斜率
// @return      void
// Sample usage:                补线，适用于十字路口的二阶段
//-------------------------------------------------------------------------------------------------------------------

void editLineMap_K(Linedata_Typedef* linedata,char direct,uint8_t yPos_start,uint8_t yPos_end,float k)
{
    //发散式补线
    float b;
    int i;
    //x = ky+b
    if(direct == 0x01){
        b = linedata->Left_line_array[yPos_start] - k*yPos_start;
    }
    else {
        b = linedata->Right_line_array[yPos_start] - k*yPos_start;
    }
    //链接
    if(yPos_start<=yPos_end){
            for(i=yPos_start;i<=yPos_end;i++){
                if(direct == LEFT_BOUNDARY)
                    linedata->Left_line_array[i] = k*i+b;
                else {
                    linedata->Right_line_array[i] = k*i+b;
                }
            }
        }
        else{
            for(i=yPos_end;i<=yPos_start;i++){
                if(direct == LEFT_BOUNDARY)
                    linedata->Left_line_array[i] = k*i+b;
                else {
                    linedata->Right_line_array[i] = k*i+b;
                }
            }
        }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中线重新合成函数
// @param       linedata:        标线结构体
// @return      void
// Sample usage:                 用于在左右补线之后合成新的中线
//-------------------------------------------------------------------------------------------------------------------
void reGenerate_MidLine(Linedata_Typedef* linedata)
{
    //重新生成图像中线
    int i;
    for(i=0;i<IMAGEWID;i++){
        linedata->Middle_line_array[i] = (linedata->Left_line_array[i]+linedata->Right_line_array[i])/2;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       十字拐点检测-上
// @param       linedata:        标线结构体
//              crossing_pos:    十字路口点位结构体
//              yPos_start:      纵坐标起点
//              yPos_end:        纵坐标终点
// @return      uint8：                              状态数据，1为至少获取一个点
// Sample usage:                十字判断的决定步骤
//-------------------------------------------------------------------------------------------------------------------

uint8_t crossing_judge_up_point(Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos,
        uint8_t yPos_start,uint8_t yPos_end){
    //上拐点特性：下边线变化大或瞬间丢失，上边线变化率小
    //限制搜索范围
    uint8 i,t;
    if(yPos_start<yPos_end){
        //搜索起点一直从下到上
        t = yPos_start;
        yPos_start = yPos_end;
        yPos_end = t;
    }
    if(yPos_end<=5) yPos_end = 5; //排除上边界数据不稳定干扰
    if(yPos_start >= IMAGEWID-5) yPos_start = IMAGEWID-5;
    //初始化点数据
    crossing_pos->Point_E[1] = 0;
    crossing_pos->Point_F[1] = 0;
    crossing_pos->Point_flag[4] = 0;
    crossing_pos->Point_flag[5] = 0;

    //开始搜素
    for(i=yPos_start;i>=yPos_end;i--){
        if(crossing_pos->Point_E[1] == 0){
            if(abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-1])<=5 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-2])<=6 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-3])<=7 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+1])>=10 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+2])>=12 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+3])>=12)
            {
                //取出角点边界做差，得到角点坐标
                crossing_pos->Point_E[1] = i;
                crossing_pos->Point_flag[4] = 1; //设置标志，本次循环不重复搜索
            }
        }
        if(crossing_pos->Point_F[1] == 0){
            if(abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-1])<=5 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-2])<=6 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-3])<=7 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+1])>=10 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+2])>=12 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+3])>=12)
            {
                //取出角点边界做差，得到角点坐标
                crossing_pos->Point_F[1] = i;
                crossing_pos->Point_flag[5] = 1; //设置标志，本次循环不重复搜索
            }
        }
        if(crossing_pos->Point_flag[4] && crossing_pos->Point_flag[5])
            break; //全部搜完，提前结束循环
    }
    //对采集的点位数据进行结算
    if(abs(crossing_pos->Point_E[1]-crossing_pos->Point_F[1])>30){
        //舍弃数据，本次搜索失败
        crossing_pos->Point_E[1] = 0;
        crossing_pos->Point_F[1] = 0;
        crossing_pos->Point_flag[4] = 0;
        crossing_pos->Point_flag[5] = 0;
        return 0;
    }
    else{
        //依据判定状态，为纵坐标赋值
        if(crossing_pos->Point_flag[4])
            crossing_pos->Point_E[0] = linedata->Left_line_array[crossing_pos->Point_E[1]];
        if(crossing_pos->Point_flag[5])
            crossing_pos->Point_F[0] = linedata->Right_line_array[crossing_pos->Point_F[1]];
        return 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       十字拐点检测-下
// @param       linedata:        标线结构体
//              crossing_pos:    十字路口点位结构体
//              yPos_start:      纵坐标起点
//              yPos_end:        纵坐标终点
// @return      uint8：                              状态数据，1为至少获取一个点
// Sample usage:                十字判断的决定步骤
//-------------------------------------------------------------------------------------------------------------------

uint8_t crossing_judge_down_point(Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos,
        uint8_t yPos_start,uint8_t yPos_end){
    //下拐点特性：上方突变或丢线，下方稳定
    //限制搜索范围
    uint8 i,t;
    if(yPos_start<yPos_end){
        //搜索起点一直从下到上
        t = yPos_start;
        yPos_start = yPos_end;
        yPos_end = t;
    }
    if(yPos_end<=5) yPos_end = 5; //排除上边界数据不稳定干扰
    if(yPos_start >= IMAGEWID-4) yPos_start = IMAGEWID-4;
    //初始化点数据
    crossing_pos->Point_C[1] = 0;
    crossing_pos->Point_D[1] = 0;
    crossing_pos->Point_flag[2] = 0;
    crossing_pos->Point_flag[3] = 0;

    //开始搜素
    for(i=yPos_start;i>=yPos_end;i--){
        if(crossing_pos->Point_C[1] == 0){
            if(abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+1])<=8 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+2])<=8 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i+3])<=8 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-1])>=15 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-2])>=15 &&
               abs(linedata->Left_line_array[i]-linedata->Left_line_array[i-3])>=15)
            {
                //取出角点边界做差，得到角点坐标
                crossing_pos->Point_C[1] = i;
                crossing_pos->Point_flag[2] = 1; //设置标志，本次循环不重复搜索
            }
        }
        if(crossing_pos->Point_D[1] == 0){
            if(abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+1])<=8 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+2])<=8 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i+3])<=8 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-1])>=15 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-2])>=15 &&
               abs(linedata->Right_line_array[i]-linedata->Right_line_array[i-3])>=15)
            {
                //取出角点边界做差，得到角点坐标
                crossing_pos->Point_D[1] = i;
                crossing_pos->Point_flag[3] = 1; //设置标志，本次循环不重复搜索
            }
        }
        if(crossing_pos->Point_flag[2] && crossing_pos->Point_flag[3])
            break; //全部搜完，提前结束循环
    }
    //对采集的点位数据进行结算
    if(abs(crossing_pos->Point_C[1]-crossing_pos->Point_D[1])>30){
        //舍弃数据，本次搜索失败
        crossing_pos->Point_C[1] = 0;
        crossing_pos->Point_D[1] = 0;
        crossing_pos->Point_flag[2] = 0;
        crossing_pos->Point_flag[3] = 0;
        return 0;
    }
    else{
        //依据判定状态，为纵坐标赋值
        if(crossing_pos->Point_flag[2])
            crossing_pos->Point_C[0] = linedata->Left_line_array[crossing_pos->Point_C[1]];
        if(crossing_pos->Point_flag[3])
            crossing_pos->Point_D[0] = linedata->Right_line_array[crossing_pos->Point_D[1]];
        return 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       左右扫描
// @param       OutImage:        二值化图像
//              xPos:            搜索的横坐标范围
//              Point:           上方的突变角点坐标，用指针返回
// @return      uint8:           1成功 0失败
// Sample usage:                 在转弯入环时，若边线角点搜索失败，会尝试继续搜索角点
//                               若此方法仍无法找到，代表已经入环
//-------------------------------------------------------------------------------------------------------------------

uint8_t L_to_R_scan(uint8_t OutImage[IMAGEWID][IMAGELEN],uint8_t xPos_Start,uint8_t xPos_Stop,
     uint8_t* Point_X,uint8_t* Point_Y)
{
    unsigned char i,j;
    unsigned char min,max;
    //从左向右进行突变点扫描
    min = (xPos_Start<xPos_Stop ? xPos_Start : xPos_Stop);
    max = (min == xPos_Start ? xPos_Stop : xPos_Start); //取反

    //L<R: 从左向右搜索
    //L>R: 从右向左搜索

    for(i=min;i<max;i++){
//        //向上找边界
//        for(j=IMAGEWID-25;j>0;j--){
//            if(OutImage[j][i] == 0xFF && OutImage[j-1][i] == 0x00){
//                Up_down_boundary[i] = j; //记录上方边界的坐标
//            }
//            else if(j==1){
//                Up_down_boundary[i] = 0;
//                break;
//            }
//        }
        //向下找边界
        for(j=5;j<IMAGEWID/2+20;j++){
            if(OutImage[j][i] == 0x00 && OutImage[j+1][i] == 0xFF){
                Up_down_boundary[i] = j; //记录边界的坐标
                break;
            }
            else if(j==1){
                Up_down_boundary[i] = IMAGEWID/2+20;
                break;
            }
        }
        //寻找突变尖点
        if(xPos_Start < xPos_Stop){
            for(i=xPos_Start;i<xPos_Stop;i++){
                if(abs(Up_down_boundary[i]-Up_down_boundary[i-1]) >= 3 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i-2]) >= 5 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i+1]) <= 2 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i+2]) <= 2)
                {
                    //记录纵坐标突变
                    *Point_X = i;
                    *Point_Y = Up_down_boundary[i];
                    return 1;
                }
            }
        }
        else
        {
            //如果起点在右侧，则反向搜索
            for(i=xPos_Start;i>=xPos_Stop;i--){
                if(abs(Up_down_boundary[i]-Up_down_boundary[i-1]) <= 2 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i-2]) <= 2 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i+1]) >= 3 &&
                   abs(Up_down_boundary[i]-Up_down_boundary[i+2]) >= 5)
                {
                    //记录突变点并退出
                    *Point_X = i;
                    *Point_Y = Up_down_boundary[i];
                    return 1;
                }
            }
        }
        //未找到突变
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       十字路口判定
// @param       OutImage:        二值化图像
//              linedata:        原始边界线数据的结构体索引
//              crossing_pos:    十字路口的四点定位参数
// @return      void
// Sample usage:                用于随时采样并将十字路口存在状态返回到crossing_flag中
//-------------------------------------------------------------------------------------------------------------------

void isCrossing(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata,CrossingPosition_Typedef* crossing_pos)
{
    unsigned char down_left_y_max,down_right_y_max; //下角点搜索限制
    //重置状态机
    crossing_flag = 0;
    crossing_stage = 0;
    //判断双侧丢线数
    Dual_missed_count=calculateMissLine(LEFT_BOUNDARY|RIGHT_BOUNDARY, linedata, IMAGEWID-1, 0);
    if(Dual_missed_count>=10)
    {
        //双侧存在大量丢线
        //寻找上侧拐点
        crossing_judge_up_point(linedata, crossing_pos, IMAGEWID-1, 0);
        if(crossing_pos->Point_flag[4] && crossing_pos->Point_flag[5]){
            //找到两个上拐点，确定存在十字
            //此处在后续版本须加入三叉的互斥判定
            crossing_flag = 1;
            crossing_stage = 1;
        }
        else return;
    }
    //找完上点后，若判断到有十字特征，则继续判断
    if(crossing_flag == 1){
        //寻找下点
        down_left_y_max = crossing_pos->Point_E[1];
        down_right_y_max = crossing_pos->Point_F[1];
        crossing_judge_down_point(linedata, crossing_pos, IMAGEWID-1,
        (down_left_y_max<down_right_y_max ? down_left_y_max : down_right_y_max));
        //取两个上角点的最小值作为搜素条件限制
        //搜素完毕，分类输出数据
        if(crossing_pos->Point_flag[2] && crossing_pos->Point_flag[3]) crossing_stage = 1;
        else if(crossing_pos->Point_flag[2] && !crossing_pos->Point_flag[3]) crossing_stage = 2;
        else if(crossing_pos->Point_flag[3] && !crossing_pos->Point_flag[2]) crossing_stage = 3;
        else crossing_stage = 4;
    }


}


//-------------------------------------------------------------------------------------------------------------------
// @brief       判定斑马线
// @param       OutImage:        二值化数组，此时将被暂时补线，直至摄像头下一次刷新
//              linedata:        标线数据，用来规定扫描范围
// @return      void
// Sample usage:                输出zebra_flag，判断斑马线存在且停车
//-------------------------------------------------------------------------------------------------------------------

void isZebra(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata){
    uint8_t down_side_count,i;
    down_side_count = 0;
    zebra_flag = 0;
    //获取前瞻的横线变化数据
    if(!crossing_flag){
        for(i=10;i<IMAGELEN-10;i++){
            if(OutImage[IMAGEWID/2][i] == 0x00 && OutImage[IMAGEWID/2][i+1] == 0xFF)
                down_side_count++;
        }
        //若两条边界线之间存在多个下降沿，判定为斑马线
        if(down_side_count >= 7){
            zebra_flag = 1;
            if(race_flag) key_value_to_process = 0x01;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       判定障碍物
// @param       OutImage:        二值化图像
// @return      void
// Sample usage:                输出obs_flag，1代表执行一次避障操作
//-------------------------------------------------------------------------------------------------------------------

void isObstruct(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata){
//    uint16 grayscale = 0;
    uint16 black_count = 0;
    uint16 white_count = 0;
    uint8_t i,j;
//    unsigned char rising,falling;
//    unsigned char rising_axis[2];
//    unsigned char falling_axis[2];
    unsigned char fall_x,fall_y;
    uint8 prejudge_flag = 0;

//    rising = 0;
//    falling = 0;
    fall_x = 0;
    fall_y = 0;

    //更新版：上溯扫描
    for(i=IMAGEWID-4;i>0;i--){
        j = linedata->Middle_line_array[i]; //记录中线对应的横坐标
        if(OutImage[i][j] == 0xFF && OutImage[i-1][j]){
            white_count++;
            continue;
        }
        else if(OutImage[i][j] == 0xFF && OutImage[i-1][j] == 0x00){
            //记录突变发生的位置
            fall_x = j;
            fall_y = i-1;
            break;
        }
     }
    //找到中线上溯的下降沿之后，按直线统计黑白点
    if(fall_x && fall_y){
        for(;i>0;i--){
            if(OutImage[i][j] == 0xFF) white_count++;
            else if(OutImage[i][j] == 0x00) black_count++;
        }
        //判断突变位置，黑点数，白点数是否达成条件
        if(fall_y >= 15 && fall_y <= 50 &&
           white_count >= 70 && black_count >= 25)
        {
            //预设条件达成
            prejudge_flag = 1;
        }
    }

//    if(obstruct_flag != 2 && race_flag == 1){ //避障操作只执行一次
//    if(obstruct_flag != 2){
//        obstruct_flag = 0;
//        //扫描图像中心6x4的像素点
//        for(i=7;i<11;i++){
//            for(j=IMAGELEN/2-3;j<IMAGELEN/2+4;j++){
//                if(OutImage[i][j] == 0x00) black_count++;
//            }
//        }
//        for(i=IMAGEWID-3;i>IMAGEWID-6;i--){
//            for(j=IMAGELEN/2-1;j<IMAGELEN/2+2;j++){
//                if(OutImage[i][j] == 0xFF) white_count++;
//            }
//        }
//
//        //判断采样区域内黑点的数量
//        if(black_count >= 15 && white_count >= 7){
//            //统计上升沿和下降沿
//            for(i=6;i<IMAGELEN-7;i++){
//                if(OutImage[8][i] == 0x00 && OutImage[8][i+1] == 0xFF){
//                    rising++;
//                    if(rising <= 2) rising_axis[rising-1] = i;
//                }
//                if(OutImage[5][i] == 0xFF && OutImage[5][i+1] == 0x00){
//                    falling++;
//                    if(falling <= 2) falling_axis[falling-1] = i;
//                }
//                //判断边沿个数
//                if(falling == 2 && rising == 2){
//                    //计算两个边界的距离
//                    if(falling_axis[0]-rising_axis[0] >= 5 &&
//                       falling_axis[1]-rising_axis[1] >= 5 &&
//                       abs((falling_axis[0]-rising_axis[0])-(falling_axis[1]-rising_axis[1])) <= 5)
//                    {
//                        //确认图像特征正确
//                        prejudge_flag = 1;
//                    }
//                }
//            }
            //判断传感器的长度
            if(Wave_distance != 0 && prejudge_flag){
                if(Wave_distance >= 3900 && Wave_distance <= 5500){
                    //开始执行避障操作
                    obstruct_flag = 1;
                }
            }
            else if(Wave_distance == 0){
                //传感去发生错误时，直接用图像进行判定
                if(prejudge_flag)
                    obstruct_flag = 1;
            }
//        }
//    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       判定直角
// @param       OutImage:        二值化图像
//              linedata：                     道路判定线数据
// @return      void
// Sample usage:                判断是否遇到直角边，进行强制转弯
//-------------------------------------------------------------------------------------------------------------------

void is90Dg(uint8 OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata){
    char direct = 0; //预判的方向
    char confirm_flag = 0; //颜色点计数标志位
    uint8 lm,rm;
//    char LR_res = 0;
    int16_t b_count = 0;
    int16_t w_count = 0;
//    uint8 i,j,x,y;
    uint8 i;
    //预先判断
    lm = calculateMissLine(LEFT_BOUNDARY, linedata, IMAGEWID-1, 0);
    rm = calculateMissLine(RIGHT_BOUNDARY, linedata, IMAGEWID-1, 0);
    if(rm >= 93 && rm-lm >= 20) direct = 2;
    else if(lm >= 93 && lm-rm >= 20) direct = 1;
    //扫描黑点数量，确定是否为直角弯
    if(direct != 0){
        for(i=0;i<IMAGELEN;i++){
            if(OutImage[5][i] == 0x00)
                b_count++;
        }
        for(i=0;i<IMAGELEN;i++){
            if(OutImage[IMAGEWID-5][i] == 0xFF)
                w_count++;
        }
        if(b_count >= 140 && w_count >= 130) confirm_flag = 1; //检测上方是否有足够的黑线
    }
//    if(bcount_flag){
//        //通过扫描边界，进一步进行判断
//        if(bcount_flag == 1){
//            LR_res = L_to_R_scan(OutImage, IMAGELEN-1, IMAGELEN-50, &x, &y);
//        }
//        else {
//            LR_res = L_to_R_scan(OutImage, 0, 50, &x, &y);
//        }
//        //获取寻找边界直角突变的结果
//        if(LR_res == 0){
//            bcount_flag = 0; //不进行90度转弯操作
//        }
//    }
    //输出结果
    if(confirm_flag==1 && direct==1) is90dg_flag = 1; //左侧的直角弯
    else if(confirm_flag==1 && direct==2) is90dg_flag = 2; //右侧的直角弯
    else is90dg_flag = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       直角转弯动作
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void quarterTurn(){
    //直角转弯的修正
    if(is90dg_flag == 1){
        motor_motion(1800, 3800, SPEED_PARAM_INIT);
        Delay_Ms(320);
    }
    if(is90dg_flag == 2){
        motor_motion(3800, 1800, SPEED_PARAM_INIT);
        Delay_Ms(320);
    }
    //重置标志位且打开摄像头
    is90dg_flag = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       判定单边桥
// @param       OutImage:        二值化图像
//              linedata：                     道路判定线数据
// @return      void
// Sample usage:                判断单边桥，按照特征更新过程变量
//-------------------------------------------------------------------------------------------------------------------

void isSingleBridge(uint8_t OutImage[IMAGEWID][IMAGELEN],Linedata_Typedef* linedata){
    #define M_ARR linedata->Middle_line_array //中线坐标重命名
    uint8 rising = 0;
    uint8 falling = 0;
    uint8 i,j;
    //未进入单边桥状态时，扫描左侧突变
    if(bridge_flag == 0){
        for(i=IMAGEWID-3;i>IMAGEWID/2+15;i--){
            if(M_ARR[i]-M_ARR[i-2] >= 8 &&
               abs(M_ARR[i]-M_ARR[i+1]) <= 2 &&
               abs(M_ARR[i]-M_ARR[i+2]) <= 3 &&
               abs(M_ARR[i-2]-M_ARR[i-3]) <= 2 &&
               abs(M_ARR[i-2]-M_ARR[i-4]) <= 3)
            {
                //判断上升沿和下降沿
                for(j=IMAGELEN-1;j>IMAGELEN/2;j--){
                    if(OutImage[60][j] == 0x00 && OutImage[60][j-1] == 0xFF) rising++;
                    if(OutImage[60][j] == 0xFF && OutImage[60][j-1] == 0x00) falling++;
                }
                //找到向左的突变位置,若之前未检测到单边桥，进入状态1
                if(falling == 1 && rising == 2){
                    tz_down = 1;
                    bridge_flag = 1;
                    bridge_stage = 1;
                    break;
                }

            }
        }
    }
    else if(bridge_flag == 1){
        //等待一阶段转弯纠正执行完成
        //扫描上突变点，判定从2阶段跳到3阶段的流程
        if(bridge_stage == 2){
            for(i=IMAGEWID-6;i>IMAGEWID/2-10;i--){
                if(M_ARR[i]-M_ARR[i-2] <= -8 &&
                   abs(M_ARR[i]-M_ARR[i+1]) <= 2 &&
                   abs(M_ARR[i]-M_ARR[i+2]) <= 3 &&
                   abs(M_ARR[i-2]-M_ARR[i-3]) <= 2 &&
                   abs(M_ARR[i-2]-M_ARR[i-4]) <= 3)
                {
                    //找到向右的突变位置,若之前未检测到单边桥，进入状态1
                    //若该点高度正常，巡线方式不变
                    if(i >= IMAGEWID-6){
                        tz_up = 1;
                        break;
                    }
                    else if(i < IMAGEWID-6){
                        //车子已经上桥，直行，放弃巡线
                        tz_up = 0;
                        bridge_stage = 3;
                        break;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       单边桥姿态调整
// @param       void
// @return      void
// Sample usage:                状态1时：右转对准 状态3时，直行下桥
//-------------------------------------------------------------------------------------------------------------------

void bridgeAdjust(void){
    if(bridge_stage == 1){ //检测到第一个突变，向右打角
        motor_motion(3800, 1300, SPEED_PARAM_INIT); //车轮右偏
        Delay_Ms(400);
        motor_motion(1500,3800,SPEED_PARAM_INIT); //回正
        Delay_Ms(300);
        motor_motion(3800, 3800, SPEED_PARAM_INIT); //直行
        Delay_Ms(200);
        bridge_stage = 2;
    }
    else if(bridge_stage == 3){
        motor_motion(2400, 2400, SPEED_PARAM_INIT); //直行过桥
        Delay_Ms(180);
        motor_motion(3000, 1400, SPEED_PARAM_INIT); //直行过桥
        Delay_Ms(140);
        //清空标志位，完成过桥
        bridge_stage = 0;
        bridge_flag = 0;
        tz_down = 0;
        tz_up = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       修补数据并重置运动参数
// @param       OutImage:        二值化数组，此时将被暂时补线，直至摄像头下一次刷新
//              crossing_pos:    十字路口的定位数据
// @return      void
// Sample usage:                接收到crossing_flag时，自动用补线的数据替换原始数据
//-------------------------------------------------------------------------------------------------------------------

void Repair_and_RefreshData(uint8_t OutImage[IMAGEWID][IMAGELEN],CrossingPosition_Typedef* crossing_pos,Linedata_Typedef* linedata)
{
    //变量定义
    float k1,k2; //左右斜率
    uint8 y_left,y_right;
    char activateflag = 0; //激活此操作的标志位
    char not_to_write_midline = 0; //禁止自动计算中线
    char not_to_write_error = 0; //禁止自动生成误差量标志位
    uint8 i;

    //对特殊情况进行分类判断
    if(crossing_flag){
      //十字路口
      activateflag = 1;
      y_left = crossing_pos->Point_E[1];
      y_right = crossing_pos->Point_F[1];
      switch(crossing_stage){
      case 1:{
          //两侧常规补线
          editLineMap_connect(linedata, 0x01, crossing_pos->Point_E[0], crossing_pos->Point_C[0],
          crossing_pos->Point_E[1], crossing_pos->Point_C[1]);
          editLineMap_connect(linedata, 0x02, crossing_pos->Point_F[0], crossing_pos->Point_D[0],
          crossing_pos->Point_F[1], crossing_pos->Point_D[1]);
          break;
      }
      case 2:{
          //右下角不存在，使用一侧斜率
          editLineMap_connect(linedata, 0x01, crossing_pos->Point_E[0], crossing_pos->Point_C[0],
          crossing_pos->Point_E[1], crossing_pos->Point_C[1]);
          //由于该直线表达式为y=kx+b，方程的x,y互换
          k2=getAverageK(y_right,
                  linedata->Right_line_array[y_right],
                  y_right-1,
                  linedata->Right_line_array[y_right-1],
                  y_right-2,
                  linedata->Right_line_array[y_right-2]);
          //k2 = (linedata->Right_line_array[crossing_pos->Point_F[1]]-
          //      linedata->Right_line_array[crossing_pos->Point_F[1]-3]) / 3.0f;
          editLineMap_K(linedata, 0x02, crossing_pos->Point_F[1], IMAGEWID-1, k2);
          break;
      }
      case 3:{
          //左下角不存在，使用一侧斜率
          editLineMap_connect(linedata, 0x02, crossing_pos->Point_F[0], crossing_pos->Point_D[0],
          crossing_pos->Point_F[1], crossing_pos->Point_D[1]);
          k1=getAverageK(y_left,
                  linedata->Left_line_array[y_left],
                  y_left-1,
                  linedata->Left_line_array[y_left-1],
                  y_left-2,
                  linedata->Left_line_array[y_left-2]);
          //k1 = (linedata->Left_line_array[crossing_pos->Point_E[1]]-
          //      linedata->Left_line_array[crossing_pos->Point_E[1]-3]) / 3.0f;
          editLineMap_K(linedata, 0x01, crossing_pos->Point_E[1], IMAGEWID-1, k1);
          break;
      }
      case 4:{
          //两侧下角点均消失，全部采用斜率补线
          k1=getAverageK(y_left,
                  linedata->Left_line_array[y_left],
                  y_left-1,
                  linedata->Left_line_array[y_left-1],
                  y_left-2,
                  linedata->Left_line_array[y_left-2]);
          k2=getAverageK(y_right,
                  linedata->Right_line_array[y_right],
                  y_right-1,
                  linedata->Right_line_array[y_right-1],
                  y_right-2,
                  linedata->Right_line_array[y_right-2]);
          editLineMap_K(linedata, 0x01, crossing_pos->Point_E[1], IMAGEWID-1, k1);
          editLineMap_K(linedata, 0x02, crossing_pos->Point_F[1], IMAGEWID-1, k2);
          break;
      }
      default:break;
      }
    }

    else if(bridge_flag == 1 && bridge_stage == 2){
        Linedata_Typedef ldata; //申请一个备份的结构体
        activateflag = 1;
        //左侧标线不变，直接复制
        for(i=0;i<IMAGEWID;i++)
            ldata.Left_line_array[i] = linedata->Left_line_array[i];
        //右侧标线，选择原有的底部右边缘进行补线
        k2 = getAverageK(IMAGEWID-7,
                         linedata->Right_line_array[IMAGEWID-7],
                         IMAGEWID-8,
                         linedata->Right_line_array[IMAGEWID-8],
                         IMAGEWID-9,
                         linedata->Right_line_array[IMAGEWID-9]
                         );
        //对新的结构体执行补线
        editLineMap_K(&ldata, RIGHT_BOUNDARY, IMAGEWID-1, 0, k2);
        //合成新的中线
        reGenerate_MidLine(&ldata);
        //用+47作为新的中线参考点，重新计算误差
        getRoadError_Default(&ldata, YPOS_REFERENCE, 47, &midLineAxis, &midLineError);
        //禁止写入误差和计算中线
        not_to_write_error = 1;
        not_to_write_midline = 1;
    }

    //调用此函数产生调用标志，重新计算数据
    if(activateflag){
        //重新合成中线
        if(not_to_write_midline == 0) reGenerate_MidLine(linedata);
        //重新解算误差数据
        if(not_to_write_error == 0) getRoadError_Default(linedata, YPOS_REFERENCE, IMAGELEN/2, &midLineAxis,&midLineError);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       清除标志位
// @param       void
// @return      void
// Sample usage:                 车辆起步时，清空所有标记，尤其是单边桥等分步不可逆标记
//-------------------------------------------------------------------------------------------------------------------

void clearFlag(){
    crossing_flag = 0;
    crossing_stage = 0;
    obstruct_flag = 0;
    bridge_flag = 0;
    bridge_stage = 0;
    is90dg_flag = 0;
    zebra_flag = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       最小二乘法求斜率
// @param       x,y:             三对拟合直线上的坐标
// @return      float:           直线斜率
// Sample usage:                 在单边补线处计算边界斜率
//-------------------------------------------------------------------------------------------------------------------

float getAverageK(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 x3,uint8 y3){
    int sum_x,sum_y,sum_xy,sum_x2;
    int m_numerator,m_denominator;
    int n=3;
    float k;

    sum_x=x1+x2+x3;
    sum_y=y1+y2+y3;
    sum_x2=x1*x1+x2*x2+x3*x3;
    sum_xy=x1*y1+x2*y2+x3*y3;

    m_numerator = n * sum_xy - sum_x * sum_y;
    m_denominator = n * sum_x2 - sum_x * sum_x;
    k = m_numerator / m_denominator;

    return k;
}
