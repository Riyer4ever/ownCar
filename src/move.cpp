#include <Arduino.h>
// 停止所有电机
void stop()
{
    analogWrite(5, 0);                      // 右侧电机停止
    analogWrite(6, 0);

    analogWrite(9, 0);                      // 左侧电机停止
    analogWrite(10, 0);
}

// 全速起步;投掷后全速前进;
void forward()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(9, 255);                    // 左侧电机逆时针旋转（向前）
}

// 上坡
void upHill()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(9, 150);                    // 左侧电机逆时针旋转（向前），因为结构问题，左侧电机需要稍微慢一点才能保持上坡直线前进
}

// 缓慢下坡
void downHill()
{
    analogWrite(6, 180);                    // 右侧电机顺时针旋转（向前）
    analogWrite(9, 120);                    // 左侧电机逆时针旋转（向前）
}

// 正常左转
void turnLeft()
{
    analogWrite(6, 120);                    // 右侧电机顺时针旋转（向前）
    analogWrite(9, 255);                    // 左侧电机逆时针旋转（向前）
}

// 正常右转
void turnRight()
{
    analogWrite(6, 255);                    // 右侧电机顺时针旋转（向前）
    analogWrite(9, 120);                    // 左侧电机逆时针旋转（向前）
}

//隧道左转（隧道里弯道更大，所以转弯的幅度要更大）
void tunnelTurnLeft()
{
    analogWrite(6, 20);                     // 右侧电机慢速顺时针旋转（向前）
    analogWrite(9, 255);                    // 左侧电机逆时针旋转（向前）
}
