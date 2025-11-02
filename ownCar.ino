// Arduino库文件引入
#include <Arduino.h>                        // Arduino核心库
#include <TimerOne.h>                       // 定时器中断库
#include <ServoTimer2.h>                    // 舵机控制库
#include "MH_TCS34725.h"                    // 颜色传感器库

// 自定义函数头文件引入
#include "./inc/move.h"                     // 运动控制相关函数
#include "./inc/gray.h"                     // 灰度传感器相关函数

// 自定义变量
int lineFlag = 0;                               // 状态标志（检测到达哪一条黑线）

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

}

void loop()
{
   
}