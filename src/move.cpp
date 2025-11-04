#include <Arduino.h>
// 停止所有电机
void stop()
{
    analogWrite(5, 0);                      // 右侧电机停止
    analogWrite(6, 0);

    analogWrite(9, 0);                      // 左侧电机停止
    analogWrite(10, 0);
}

// 全速起步；全速过大坑;
void forward()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(5, 0);
    analogWrite(10, 255);                   // 左侧电机逆时针旋转（向前）
    analogWrite(9, 0);
}

// 上坡
void upHill()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(5, 0);

    analogWrite(10, 255);                   // 左侧电机逆时针旋转（向前）
    analogWrite(9, 0);
}

// 缓慢下坡
void downHill()
{
    analogWrite(6, 100);                    // 右侧电机顺时针旋转（向前）
    analogWrite(5, 0);

    analogWrite(10, 100);                   // 左侧电机逆时针旋转（向前）
    analogWrite(9, 0);
}

// 正常左转
void turnLeft()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(5, 0);

    analogWrite(10, 0);
    analogWrite(9, 0);                    // 左侧电机逆时针旋转（向前）
}

// 正常右转
void turnRight()
{
    analogWrite(5, 0);                    // 右侧电机顺时针旋转（向前）
    analogWrite(6, 0);

    analogWrite(10, 255);                   // 左侧电机逆时针旋转（向前）
    analogWrite(9, 0);
}

// 灵敏左转
void turnLeftPower()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(5, 0);

    analogWrite(10, 0);
    analogWrite(9, 255);                    // 左侧电机逆时针旋转（向前）
}

// 灵敏右转
void turnRightPower()
{
    analogWrite(5, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(6, 0);

    analogWrite(10, 255);                   // 左侧电机逆时针旋转（向前）
    analogWrite(9, 0);
}