/*********************************************************************************************************************
* @file            main.c
* @brief           程序模板
* @author          lemon
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-3-2
*
********************************************************************************************************************/

/********************************************详细说明********************************************

* 1. 如果需要从外面调库函数，只需将所用到的库函数放在CODE文件中，在工程中添加即可成功编译
* 2. 对于用户而言，只需要更改main函数的内容即可实现想要的功能
* 3. 如果需要声明变量或宏定义内容，可以放在main.h文件中
* 4. 如果需要用到中断，中断服务函数可以放在isr.c文件中
* 5. 如果需要写一些说明性的文档，可以放在doc文件夹的readme中
*
************************************************************************************************/


/* 用户头文件 */
#include "main.h"
#include "headfile.h"

/* 全局变量声明 */
uint8 Pixle[MT9V03X_DVP_H][MT9V03X_DVP_W];

#define hangup 40
#define hangdown 115
char key_flag3;
char pict[hangdown-hangup][MT9V03X_DVP_W];
//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车最大类方差法求阈值
// @param        image像素数据，行数列数
// @return       灰度阈值
//-------------------------------------------------------------------------------------------------------------------
int my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
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
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {

                w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
                u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;

                u0 = u0tmp / w0;              //背景平均灰度
                u1 = u1tmp / w1;              //前景平均灰度
                u = u0tmp + u1tmp;            //全局平均灰度
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);//平方
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;//最大类间方差法
                    threshold = j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }
         }
    return threshold;

}
//-------------------------------------------------------------------------------------------------------------------
// @brief        计算偏差值，边界线，中线
// @param        起始行、结束行和步长
// @return       赛道中心线相对于图像中心的偏移量
//-------------------------------------------------------------------------------------------------------------------
typedef struct   //定义一个结构体，用于放置所用点的所在列数
{
    int column;
}usefulpoint;
usefulpoint jump_left[MT9V03X_DVP_H], jump_right[MT9V03X_DVP_H], midpoint[MT9V03X_DVP_H];//定义每一行左，右跳变点，赛道中点

double camera_deviation(int start_row, int end_row, int step)
{
    int column, row,num_mid = 0;     //定义检测偏差中所需的中间变量及标志
    double deviation=0;
/////////////////////
//////////////////////

        for(row = end_row-1; row >= start_row; row=row-step){
            //左边边界
            jump_left[row].column = 0;

            if(row < end_row -1 && jump_left[row+1].column > 0){//找到下方的一个点,上一个点不是左边界
                if(Pixle[row][jump_left[row+1].column] == 0){   //判断当前的点,白点
                    for(int i = jump_left[row+1].column; i >= 2;i--){
                        if((Pixle[row][i-2]==255) && (Pixle[row][i-1]==255)){
                            jump_left[row].column = i-1;    //记录左侧跳变点列数
                            break;
                         }
                    }
                }else{   //当前点是黑点
                    for(int i = jump_left[row+1].column; i < MT9V03X_DVP_W - 2; i++){
                        if((Pixle[row][i+1]==0) && (Pixle[row][i+2]==0)){
                            jump_left[row].column = i;    //记录左侧跳变点列数
                            break;
                        }
                    }
                    if(jump_left[row +1].column>=MT9V03X_DVP_W - 3){
                        jump_left[row].column = MT9V03X_DVP_W - 1;
                    }
                }

            }else{
                for(column = MT9V03X_DVP_W/2; column >= 1; column--){
                    if ( (Pixle[row][column-1]==255) && (Pixle[row][column]==255) && (Pixle[row][column+1]==0) && (Pixle[row][column+2]==0))
                    {
                        jump_left[row].column = column;    //记录左侧跳变点列数
                        break;                             //找到左侧跳变点，跳出循环
                    }
                }
            }

            //右边边界
            jump_right[row].column = MT9V03X_DVP_W-1;

            if(row < end_row -1 && jump_right[row+1].column < MT9V03X_DVP_W-1){//找到下方的一个点,上一个点不是左边界
                if(Pixle[row][jump_right[row+1].column] == 0){   //判断当前的点,白点
                    for(int i = jump_right[row+1].column; i <MT9V03X_DVP_W - 2;i++){
                        if((Pixle[row][i+2]==255) && (Pixle[row][i+1]==255)){
                            jump_right[row].column = i + 1;    //右侧边界赋值
                            break;
                         }
                    }
                }else{   //当前点是黑点
                    for(int i = jump_right[row+1].column; i >= 2; i--){
                        if((Pixle[row][i-2]==0) && (Pixle[row][i-1]==0)){
                            jump_right[row].column = i;    //记录右侧跳变点列数
                            break;
                        }
                    }
                    if(jump_right[row +1].column<=2){
                        jump_right[row].column = 0;
                    }
                }

            }else{
                for(column = MT9V03X_DVP_W/2; column <= MT9V03X_DVP_W - 2; column++){
                    if ( (Pixle[row][column-2]==0) && (Pixle[row][column-1]==0) && (Pixle[row][column]==255) && (Pixle[row][column+1]==255))
                    {
                        jump_right[row].column = column;    //记录右侧跳变点列数
                        break;                             //找到右侧跳变点，跳出循环
                    }
                }
            }
        }


    for (row = start_row ; row < end_row ; row=row+step)
    {

       midpoint[row].column = (jump_left[row].column+jump_right[row].column)/2;  //计算赛道中点所在列
       num_mid++;
                                                      //赛道中点个数增加
     }
        //计算偏差1.0
     for (row = start_row ; row < end_row ; row=row+step)
     {

         deviation = deviation + midpoint[row].column - MT9V03X_DVP_W/2;

     }
     deviation = deviation/num_mid;
//  key_flag3==0进行整幅图二值化处理
     if(key_flag3==0)
     {
     for(row=start_row;row<end_row;row++)
     {
         for(column=0;column<MT9V03X_DVP_W;column++)
         {
             pict[row-start_row][column]=0;
         }
     }
     for(row=start_row;row<end_row;row++)
     {
         pict[row-start_row][jump_left[row].column]=255;
         pict[row-start_row][jump_right[row].column]=255;
     }
     }
     return deviation;
}

/* 主函数 */
int main(void)
{
    car_init();

    while(1)
    {

    }

}


