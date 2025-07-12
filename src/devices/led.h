#ifndef LED_H
#define LED_H

#include <Arduino.h>

// LED管理函数
void initLED(int pin);
void setLED(int pin, bool state);
void toggleLED(int pin);
void blinkLED(int pin, int times, int delayMs);
void blinkLEDAsync(int pin, int times, int delayMs); // 非阻塞闪烁
void updateLEDs(); // 处理异步LED更新，需要在主循环中调用

// LED状态查询
bool getLEDState(int pin);

#endif
