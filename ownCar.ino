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
    Timer1.attachInterrupt(grayDetect_ISR); // 设置定时器中断，50ms检测一次灰度传感器

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

    // 完成隧道后，检测到第二条黑线，进行色卡检测（不停车检测色卡）
    else if (lineFlag == 2)
    {
        // 检测标准色卡
        noInterrupts();                     // 关闭中断，执行色卡检测动作
        stop();                             // 停止
        delay(500);                         // 延时500ms，稳定
        interrupts();                       // 开启中断，继续灰度检测
        delay(0);                           //停车位到标准色卡位置的延时
        colorFlag = colorDetect(0);         // 颜色检测函数，返回颜色标志
        

        // 检测第一色卡
        delay(0);                           // 标准色卡到后方第一色卡位置的延时
        colorCompare = colorDetect(0);      // 颜色检测函数，返回颜色标志
        if (colorFlag == colorCompare)
        {
            // 放下物块
            delay(0);                       // 延时，到达放下位置
            noInterrupts();                 // 关闭中断，执行放下动作
            stop();                         // 停止
            delay(500);                     // 延时500ms，稳定
            servo3.write(0);                // 机械臂旋转到合适位置
            servo2.write(0);                // 放下机械臂
            delay(0);                       // 延时等待机械臂到合适位置再放下
            servo1.write(0);                // 放开物块
            delay(0);                       // 等待放下完成

            // 机械臂复位
            servo3.write(0);
            servo2.write(0);
            interrupts();                   // 开启中断，继续灰度检测
        }

        else
        {
            delay(0);                       // 第一色卡到后方第二色卡位置的延时
        }

        // 检测第二色卡
        colorCompare = colorDetect(0);      // 颜色检测函数，返回颜色标志
        if (colorFlag == colorCompare)
        {
            // 放下物块
            delay(0);                       // 延时，到达放下位置
            noInterrupts();                 // 关闭中断，执行放下动作
            stop();                         // 停止
            delay(500);                     // 延时500ms，稳定
            servo3.write(0);                // 机械臂旋转到合适位置
            servo2.write(0);                // 放下机械臂
            delay(0);                       // 延时等待机械臂到合适位置再放下
            servo1.write(0);                // 放开物块
            delay(0);                       // 等待放下完成

            // 机械臂复位
            servo3.write(0);
            servo2.write(0);
            interrupts();                   // 开启中断，继续灰度检测
        }

        else
        {
            delay(0);                       // 第二色卡到后方第三色卡位置的延时
        }

        // 检测第三色卡
        colorCompare = colorDetect(0);      // 颜色检测函数，返回颜色标志
        if (colorFlag == colorCompare)
        {
            // 放下物块
            delay(0);                       // 延时，到达放下位置
            noInterrupts();                 // 关闭中断，执行放下动作
            stop();                         // 停止
            delay(500);                     // 延时500ms，稳定
            servo3.write(0);                // 机械臂旋转到合适位置
            servo2.write(0);                // 放下机械臂
            delay(0);                       // 延时等待机械臂到合适位置再放下
            servo1.write(0);                // 放开物块
            delay(0);                       // 等待放下完成

            // 机械臂复位
            servo3.write(0);
            servo2.write(0);
            interrupts();                   // 开启中断，继续灰度检测
        }

        else
        {
            noInterrupts();                 // 关闭中断，执行停止动作
            stop();                         // 三个色卡均未匹配成功，停止
        }
    }

    // 检测到第三条黑线，准备过大坑
    else if (lineFlag == 3)
    {
        delay(0);                           // 行进到大坑边缘的延时
        noInterrupts();                     // 关闭中断，执行过大坑动作
        forward();                          // 全速过大坑
        delay(0);                           // 延时，完成过大坑
        interrupts();                       // 开启中断，继续灰度检测
        delay(0);                           // 延时，出大坑后循迹
        noInterrupts();                     // 到达终点关闭中断，执行停止动作
        stop();                             // 停止
    }

    else
    {
        noInterrupts();                     // 关闭中断
        stop();                             // 其他情况停止不动
    }

}