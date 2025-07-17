#ifndef SCREEN_H
#define SCREEN_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // 没有接复位引脚
#define SCREEN_ADDRESS 0x3C // 常见默认I2C地址

// 声明全局display对象（在screen.cpp中定义）
extern Adafruit_SSD1306 display;
// 声明U8g2对象用于中文显示
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// 屏幕管理函数
void initScreen();

// 省电相关函数
void turnOffScreen();         // 软件关闭显示
void turnOnScreen();          // 重新开启显示
void setScreenBrightness(uint8_t brightness); // 设置亮度 (0-255)
void screenSleep();           // 进入睡眠模式  
void screenWakeup();          // 从睡眠模式唤醒
void displayText(String text, int x = 0, int y = 15); // 显示文本的便捷函数（清除屏幕）
void addText(String text, int x = 0, int y = 15); // 在现有内容基础上添加文本
void clearScreen(); // 清除屏幕内容
#endif
