#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// 按钮事件回调函数类型定义
typedef void (*ButtonPressCallback)();

// 按钮管理函数
void initButton(ButtonPressCallback callback);
void checkButton();

// 如果需要获取按钮状态，可以添加getter函数
bool isButtonPressed();
unsigned long getLastButtonPressTime();

#endif
