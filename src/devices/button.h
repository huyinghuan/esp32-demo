#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// 按钮管理函数
void initButton();
void checkButton();
void sendButtonPressMessage();

// 如果需要获取按钮状态，可以添加getter函数
bool isButtonPressed();
unsigned long getLastButtonPressTime();

// 表情状态管理
int getCurrentEmojiType();
void switchToNextEmoji();

#endif
