#include "main.h"
#include "headfile.h"

/* 全局变量声明 */
int times = 8;
int a = 0;
double b = 0;
int zhuanup = 0;
int now = 0;
char zhangai_flag=0;
char flag3 = 1;
char flag10=0,flag11=0,flag12=0,flag13=0,chuhuan,rukuuu = 0;//角度检测标志
uint8 Pixle[MT9V03X_DVP_H][MT9V03X_DVP_W];
#define hangup 40
#define hangdown 115
int key_flag3 = 0;
char pict[hangdown-hangup][MT9V03X_DVP_W];


///////////////
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


///////////////
typedef struct   //定义一个结构体，用于放置所用点的所在列数以及标志符
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
////////////////////
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
////////////////////
     return deviation;
}
//////////////////////////////////////////////




////////////////////////////////////////////
void jiaojiance()
{
    int hang1 = 0;
    flag10=0,flag11=0,flag12=0,flag13=0;
    chuhuan = 0;
    rukuuu = 0;
    char ruku_biaozhi = 0;
    int you1zhi = 0,you2zhi = 0,zuo1zhi = 0;
    for(hang1 = hangup+10;hang1<hangdown-10;hang1++)
    {
        if(jump_left[hang1].column-jump_left[hang1-3].column>25)
        {
//            ips114_show_string(210,80,"zuo2");
            flag10=1;
        }
        if(jump_left[hang1-5].column-jump_left[hang1].column>30&&(jump_left[hang1-7].column-jump_left[hang1-5].column<5)&&jump_left[hang1+3].column-jump_left[hang1].column<10)
        {
//            ips114_show_string(210,80,"zuo1");
            zuo1zhi = hang1;
            flag11=1;
        }
        if(jump_right[hang1].column-jump_right[hang1-5].column>25)
        {
//            ips114_show_string(210,60,"you1");
            flag12=1;
            you1zhi = hang1;
        }

        if((jump_right[hang1-2].column-jump_right[hang1+3].column>25)||(jump_right[hang1-2].column-jump_right[hang1+2].column>20))
        {
            flag13=1;
//            ips114_show_string(210,60,"you2");
            you2zhi = hang1;
        }

//        if(flag12*flag13)//扫到右两个点入库
//        {
//
//            ips114_show_string(210,40,"ruku");
//        }
        if(jump_right[hang1].column-jump_left[hang1].column < 20 && hang1 > 60){
            rukuuu = 1;
            ips114_show_string(210,40,"ruku");
        }
        if(you2zhi>70)//右2点左转出环
        {
            chuhuan = 1;
            ips114_show_string(210,40,"cuhu");
        }
//       if(!(flag10&&flag11&&flag12&&flag13))
//        {
//           now = 600;
////            ips114_show_string(210,80,"----");
////            ips114_show_string(210,60,"----");
////            ips114_show_string(210,40,"----");
//
//        }
        if(flag10*flag13)
        {
            ips114_show_string(210,40,"shzi");
            now=3200;
        }
        if(flag11*flag12)
        {
            ips114_show_string(210,40,"shzi");
            now=3200;
        }
        if((rukuuu == 1||(flag10==1&&flag11==1))&&zuo1zhi>=50)
        {
            now =600;

        }
        if(hang1<50)
            flag11 =0;
    }
}

void ruku()
{
//    if(rukuuu == 1)//||(flag12==1&&flag13==1))//右入库
//    {
//
//        motor_back(left,0);//智能车停止
//        motor_back(right,0);
//        Delay_Ms(15);
//        motor_back(right, 2500);
//        motor_forward(left, 2500);
//        Delay_Ms(300);
//        motor_forward(left,2500);
//        motor_forward(right,2500);
//        Delay_Ms(600);
//        motor_back(left,0);//智能车停止
//        motor_back(right,0);
//        Delay_Ms(15);
//        motor_forward(left,2500);
//        motor_forward(right,1500);
//        Delay_Ms(250);
//        motor_forward(left,2500);
//        motor_forward(right,2000);
//        Delay_Ms(150);
//        motor_back(left,0);//智能车停止
//        motor_back(right,0);
//        Delay_Ms(15);
//        flag3 = 0;
//        //while(1);
//    }
    if(rukuuu == 1)//if(flag12==1&&flag13==1)//左入库
    {

        motor_back(left,0);//智能车停止
        motor_back(right,0);
        Delay_Ms(15);
        motor_back(left, 2500);
        motor_forward(right, 2500);
        Delay_Ms(300);
        motor_forward(left,2500);
        motor_forward(right,2500);
        Delay_Ms(600);
        motor_back(left,0);//智能车停止
        motor_back(right,0);
        Delay_Ms(15);
        motor_forward(right,2500);
        motor_forward(left,1500);
        Delay_Ms(250);
        motor_forward(left,2500);
        motor_forward(right,2000);
        Delay_Ms(150);
        motor_back(left,0);//智能车停止
        motor_back(right,0);
        Delay_Ms(15);
        flag3 = 0;
        //while(1);
    }
}

void huandaozhixing()
{
   if(flag11)
    {
       motor_forward(left,0);//智能车停止
       motor_forward(right,0);
       Delay_Ms(15);
        motor_back(left,0);
        motor_forward(right,2500);    //左转3
        Delay_Ms(310);
        now = 200;
//        motor_forward(left,0);//智能车停止
//        motor_forward(right,0);
//        Delay_Ms(15);
    }
   if(chuhuan)
   {
       motor_forward(left,0);
       motor_forward(right,2800);    //左转3
       Delay_Ms(230);
       now = 3200;
       motor_forward(left,0);//智能车停止
       motor_forward(right,0);
       Delay_Ms(15);
   }

}

/////////////////////////////////////////////
void Zhang(void){
    int row = hangup+15;//+8
        for(;row<hangup + 30;row++)
        {
            if((jump_right[row-4].column==187 && jump_right[row-3].column==187&&jump_right[row-1].column<=168) || (jump_right[row-2].column - jump_right[row-5].column > 50))
            {
                int i = row - 7;
                for(; i < row + 8; i++)
                {
                    if(jump_left[i].column == 0&&jump_left[i+1].column==0&&jump_left[i+2].column>20 && jump_left[i + 8].column > 5)
                    {
                        if(Pixle[i+4][94] == 255)
                        {
                            ips114_show_float(210,65,25,2,0);
                            motor_back(left,0);//智能车停止
                            motor_back(right,0);
                            Delay_Ms(15);
                            zhangai_flag=1;

                            return;
                        }
                    }
                }
            }
        }

}
//////////////
/* 主函数 */
int main(void)
{

    /* 此处声明需要用到的局部变量 */
    unsigned char key_value;
    char key_flag1 = 0;
    char key_flag2 = 0;

    /* 智能车初始化 */
    car_init();


    /* 此处编写单次运行的代码(例如：初始化代码等) */
    /////////////////
    exti_enable(EXTI_Line3, C3, BOTH);
    exti_enable(EXTI_Line4, C4, BOTH);
    exti_enable(EXTI_Line5, C5, BOTH);

    ips114_init();     //初始化屏幕
    ips114_show_string(0,0,"CAM-IPS");
    mt9v03x_init_dvp();
    ips114_show_string(0,20,"DEMO");

    Delay_Ms(100);

    while(1)
    {


        key_value = key_check();

        if (key_value != 0xFF)
        {
            if(key_value == 0x06)
            {
                //按下按键6，
                key_flag3 = 1;
                ips114_show_string(208, 80, "key6");
            }
            else if(key_value == 0x05)
            {
                //按下按键5，
                ips114_show_string(208, 80, "key5");
                key_flag3 = 0;
            }

            else if(key_value == 0x02)
            {
                //按下按键2，
                ips114_show_string(208, 80, "key2");
                key_flag1 = 2;
            }
            else if(key_value == 0x04)
            {
                //按下按键4，
                ips114_show_string(208, 65, "key3");
                key_flag2 = 2;
                key_flag1 = 2;
            }

        }

        if(key_flag1>=1)
        {
            if(now>=1)
            now--;

            ips114_show_string(210,0,"DEMT");

            if (mt9v03x_finish_flag_dvp == 1)
            {
                if (times >= 7)
                {
                    a = my_adapt_threshold(mt9v03x_image_dvp[0],MT9V03X_DVP_W,MT9V03X_DVP_H);
                    times =0;
                }

                for(int i=0;i<MT9V03X_DVP_H;i++)
                {
                    for(int j=0;j<MT9V03X_DVP_W;j++)
                    {
                        if(mt9v03x_image_dvp[i][j]<a)
                        {
//                            mt9v03x_image_dvp[i][j]=255;
                            Pixle[i][j]=255;


                        }
                        else
                        {
//                            mt9v03x_image_dvp[i][j]=0;
                            Pixle[i][j]=0;
                        }
                    }
                }
                //二值化计数
                times++;
                //显示图像
                if(key_flag3)
                {
                ips114_show_gray_image(0,0,Pixle[0],MT9V03X_DVP_W,MT9V03X_DVP_H,MT9V03X_DVP_W,MT9V03X_DVP_H,0);
                }
                else
                {
                ips114_show_gray_image(0,hangup,pict[0],MT9V03X_DVP_W,hangdown-hangup,MT9V03X_DVP_W,hangdown-hangup,0);
                }
                //显示检测范围
                ips114_draw_line(190,hangup,210,hangup,RGB565_RED);
                ips114_draw_line(190,hangdown,210,hangdown,RGB565_RED);
                ips114_draw_line(midpoint[hangup+8].column,hangup+8,midpoint[hangdown-8].column,hangdown-8,RGB565_RED);
                ////偏离值显示
                b = camera_deviation(hangup,hangdown,1);
//                Delay_Ms(10);
                ips114_show_float(210,115,b,3,0);
                ips114_show_float(206,100,now,6,0);

                //判断环岛
                jiaojiance();

                if(flag3)//电动机控制
                {

                 if(-5<=b&&b<=5)
                 {
                     motor_forward(left,4000);
                     motor_forward(right,3998);      //智能车前进
                     Delay_Ms(10);
                 }
                 else if (b>=5&&b<=10)
                 {
                     motor_forward(left,3600);
                     motor_forward(right,3450);      //
                     Delay_Ms(10);
                }
                 else if (b>=-10&&b<-5)
                 {
                     motor_forward(left,3400);
                     motor_forward(right,3600);      //
                     Delay_Ms(10);
                }
                 else if (b>=-15&&b<-10)
                 {
                     motor_forward(left,3000);
                     motor_forward(right,3600);      //
                     Delay_Ms(10);
                }
                 else if(b>=-20&&b <-15)
                 {
                     motor_forward(left,2800);
                     motor_forward(right,3600);    //左转0
                     Delay_Ms(10);
                 }
                 else if(b>=-40&&b<-20)
                 {
                     motor_forward(left,2600);
                     motor_forward(right,3600);    //左转1
                     Delay_Ms(10);
                 }
                 else if(b>=-45&&b<-40)
                 {
                     motor_forward(left,2200);
                     motor_forward(right,3600);    //左转1
                     Delay_Ms(10);
                 }

                 else if(b>=-50&&b<-45)
                 {
                     motor_forward(left,2100);
                     motor_forward(right,3800);    //左转2
                     Delay_Ms(10);
                 }
                 else if(b>-65&&b<-50)
                 {
                     motor_forward(left,1700);
                     motor_forward(right,3800);    //左转2
                     Delay_Ms(10);
                 }


                 else if(b>10&&b<=20)
                 {
                     motor_forward(left,3800);      //  右转1
                     motor_forward(right,3000);
                     Delay_Ms(10);
                 }
                 else if(b>20&&b<=30)
                 {
                     motor_forward(left,3750);      //  右转1
                     motor_forward(right,2750);
                     Delay_Ms(10);
                 }
                 else if(b>30&&b<45)
                 {
                     motor_forward(left,3800);      //  右转2
                     motor_forward(right,2600);
                     Delay_Ms(10);
                 }
                 else if(b>45&&b<60)
                 {
                     motor_forward(left,3800);      //  右转3
                     motor_forward(right,2000);
                     Delay_Ms(10);
                 }

                 else
                 {
                     motor_forward(left,0);//智能车停止
                     motor_forward(right,0);
                     Delay_Ms(15);
                 }
                 zhuanup = 0;
                 if(now==0)
                 {
                  huandaozhixing();
                 }
              }
            }
            mt9v03x_finish_flag_dvp = 0;

        }
    }
}




