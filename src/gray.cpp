// 使用定时器中断检测灰度传感器数值
#include <Arduino.h>                        // Arduino核心库
#include "../inc/move.h"                    // 运动控制相关函数
#include <Wire.h>                           // I2C通信库

extern int lineFlag;                        // 声明状态标志变量

void grayDetect_ISR()
{
    int rightGray = 0;                      // 右灰度传感器数值
    int leftGray = 0;                       // 左灰度传感器数值

    rightGray = analogRead(A0);             // 读取右侧灰度传感器数值
    leftGray = analogRead(A2);              // 读取左侧灰度传感器数值

    // Serial.println(rightGray);
    // Serial.println(leftGray);
    // delay(1000);


    // 两侧均未检测到黑线，前进
    if (rightGray > 400 && leftGray > 400)
    {
        forward();
    }
    
    // 右侧检测到黑线，左侧未检测到，右转
    else if (rightGray < 400 && leftGray > 400 && lineFlag == 0)
    {
        turnRight();
    }

    // 左侧检测到黑线，右侧未检测到，左转
    else if (rightGray > 400 && leftGray < 400 && lineFlag == 0)
    {
        turnLeft();
    }

    else if (rightGray < 400 && leftGray > 400 && lineFlag != 0)
    {
        turnRightPower();
    }

    else if (rightGray > 400 && leftGray < 400 && lineFlag != 0)
    {
        turnLeftPower();
    }

    // 两侧均检测到黑线，更新状态标志
    else
    {
        lineFlag++;
    }
}