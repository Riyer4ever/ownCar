// 使用定时器中断检测灰度传感器数值
#include <Arduino.h>

void grayDetect()
{
    int rightGray = 0;                          // 右灰度传感器数值
    int leftGray = 0;                           // 左灰度传感器数值
    rightGray = analogRead(A0);             // 读取右侧灰度传感器数值
    leftGray = analogRead(A2);              // 读取左侧灰度传感器数值
}