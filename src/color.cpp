#include "MH_TCS34725.h"                    // 颜色传感器库 

// 颜色检测函数，返回颜色标志；1-红色，2-绿色，3-蓝色，0-未知颜色，delayTime-延时参数，单位毫秒，用于不停车检测中避免数据冲突
int colorDetect(int delayTime)
{
    uint16_t r, g, b, c;                    // 颜色传感器的原始数据变量
    uint8_t R, G, B;                        // 颜色传感器处理后的数据变量
    int color = 0;                          // 默认颜色未知

    tcs.getRGBC(&r, &g, &b, &c);            // 获取颜色传感器的原始数据
    tcs.lock();                             // 锁定当前数据，防止在处理过程中数据变化 
    

    if (c > 0)
    {
        R = (uint8_t)((float)r / c * 255.0);   // 计算处理后的红色数据
        G = (uint8_t)((float)g / c * 255.0);   // 计算处理后的绿色数据
        B = (uint8_t)((float)b / c * 255.0);   // 计算处理后的蓝色数据
    }

    tcs.unlock();                           // 解锁数据

    if (R > G && R > B)                     // 红色判断条件
    {
        color = 1;                          // 识别为红色
    }
    else if (G > R && G > B)                // 绿色判断条件
    {
        color = 2;                          // 识别为绿色
    }
    else if (B > R && B > G)                // 蓝色判断条件
    {
        color = 3;                          // 识别为蓝色
    }
    else
    {
        color = 0;                          // 未识别颜色
    }

    delay(delayTime);                       // 延时，确保数据处理完成，避免冲突
    
    return color;                           // 返回识别结果
}