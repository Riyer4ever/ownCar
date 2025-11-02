// Arduino库文件引入
#include <Arduino.h>                        // Arduino核心库
#include <TimerOne.h>                       // 定时器中断库
#include <ServoTimer2.h>                    // 舵机控制库
#include <Wire.h>                           // I2C通信库

// 自定义函数头文件引入
#include "./inc/move.h"                     // 运动控制相关函数
#include "./inc/gray.h"                     // 灰度传感器相关函数
#include "./inc/color.h"                    // 颜色传感器相关函数

// 自定义变量
int lineFlag = 0;                           // 状态标志（检测到达哪一条黑线）
int colorFlag = 0;                          // 颜色标志（检测到哪种颜色）；1-红色，2-绿色，3-蓝色，0-未知颜色
int colorCompare = 0;                       // 后方色卡的颜色标志（与colorFlag对比，判断是否正确）

// 创建舵机对象
ServoTimer2 servo1;                         // 夹爪舵机
ServoTimer2 servo2;                         // 抬臂舵机
ServoTimer2 servo3;                         // 旋转舵机

//设置颜色传感器采样周期50毫秒
MH_TCS34725 tcs = MH_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
    //配置引脚模式
    pinMode(A0, INPUT);                     // 右侧灰度传感器
    pinMode(A2, INPUT);                     // 左侧灰度传感器
    pinMode(5, OUTPUT);                     // 右侧电机逆时针旋转（向后）
    pinMode(6, OUTPUT);                     // 右侧电机顺时针旋转（向前）
    pinMode(9, OUTPUT);                     // 左侧电机逆时针旋转（向前）
    pinMode(10, OUTPUT);                    // 左侧电机顺时针旋转（向后）

    // 舵机引脚配置
    servo1.attach(3);                       // 夹爪舵机
    servo2.attach(12);                      // 抬臂舵机
    servo3.attach(11);                      // 旋转舵机

    // 初始化舵机位置
    servo1.write(0);                        // 旋转舵机初始化
    servo2.write(0);                        // 抬臂舵机初始化
    servo3.write(0);                        // 夹爪舵机初始化
    delay(500);

    // 初始夹取
    servo3.write(0);                        // 机械臂旋转到合适位置
    servo2.write(0);                        // 放下机械臂
    servo1.write(0);                        // 夹取物块
    delay(0);                               // 等待夹取完成
    servo2.write(0);                        // 抬起机械臂
    delay(0);                               // 等待抬起完成

    Timer1.initialize(50000);               // 初始化定时器，设置周期为50ms
    Timer1.attachInterrupt(grayDetect);     // 设置定时器中断，50ms检测一次灰度传感器

    Serial.begin(9600);                     // 初始化串口，设置波特率为9600
}

void loop()
{
    // 检测到第一条黑线，开始过坡
    if (lineFlag == 1)
    {
        noInterrupts();                     // 关闭中断，执行上坡动作
        upHill();                           // 上坡
        delay(2800);                        // 延时3400ms，完成上坡
        stop();                             // 停止
        delay(500);                         // 延时500ms，稳定
        downHill();                         // 缓慢下坡
        delay(1500);                        // 延时2000ms，完成下坡
        interrupts();                       // 开启中断，继续灰度检测
    }

    // 完成隧道后，进行色卡检测（不停止检测色卡）
    else if (lineFlag == 2)
    {
        // 检测标准色卡
        noInterrupts();                     // 关闭中断，执行色卡检测动作
        stop();                             // 停止
        delay(500);                         // 延时500ms，稳定
        forward();                          // 前进，检测标准色卡
        delay(0);                           // 延时，到达第一个色卡位置
        interrupts();                       // 开启中断，继续灰度检测
        colorFlag = colorDetect(0);         // 颜色检测函数，返回颜色标志，此处的延时是从标准色卡到达第一个色卡的时间

        // 检测后方色卡
        colorCompare = colorDetect(0);      // 颜色检测函数，返回颜色标志，此处的延时是从第一个色卡到达第二个色卡的时间
        if (colorFlag == colorCompare)
        {
            noInterrupts();                 // 关闭中断，执行放下动作
            forward();                      // 前进，达到放下位置
            delay(0);                       // 延时，到达放下位置
            stop();
        }
    }
}