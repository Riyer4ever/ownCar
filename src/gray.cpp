// 使用定时器中断检测灰度传感器数值
#include <Arduino.h>                        // Arduino核心库
#include "../inc/move.h"                    // 运动控制相关函数

extern int lineFlag;                        // 声明状态标志变量

void grayDetect()
{
    int rightGray = 0;                      // 右灰度传感器数值
    int leftGray = 0;                       // 左灰度传感器数值

    rightGray = analogRead(A0);             // 读取右侧灰度传感器数值
    leftGray = analogRead(A2);              // 读取左侧灰度传感器数值

    // 右侧检测到黑线，左侧未检测到，右转
    if (rightGray < 200 && leftGray > 200)
    {
        turnRight();
    }

    // 左侧检测到黑线，右侧未检测到，左转
    else if (rightGray > 200 && leftGray < 200)
    {
        turnLeft();
    }
    
    // 两侧均未检测到黑线，前进
    else if (rightGray > 200 && leftGray > 200)
    {
        forward();
    }

    // 两侧均检测到黑线，更新状态标志
    else
    {
        lineFlag++;
    }
}