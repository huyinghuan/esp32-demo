#include "button.h"
#include "led.h"
#include "screen.h"  // 添加screen头文件

// 按钮状态变量（模块内部私有）
static bool buttonPressed = false;
static int lastButtonState = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long lastButtonPressTime = 0;

// 按钮按下事件回调函数指针
static ButtonPressCallback buttonPressCallback = nullptr;

int _buttonPin = 0;
const unsigned long debounceDelay = 50;

void initButton(int buttonPin, ButtonPressCallback callback) {
  _buttonPin = buttonPin;
  buttonPressCallback = callback;
  pinMode(buttonPin, INPUT_PULLUP);
  buttonPressed = false;
  lastButtonState = HIGH;
  lastDebounceTime = 0;
  lastButtonPressTime = 0;
}

void checkButton() {
  int currentState = digitalRead(_buttonPin);
  
  // 检测按钮从高电平变为低电平（按下）
  if (lastButtonState == HIGH && currentState == LOW) {
    buttonPressed = true;
    lastDebounceTime = millis();
    //Serial.println("Button pressed down");
  }
  
  // 检测按钮从低电平变为高电平（松开）
  else if (lastButtonState == LOW && currentState == HIGH && buttonPressed) {
    // 防抖处理
    if (millis() - lastDebounceTime > debounceDelay) {
      buttonPressed = false;
      lastButtonPressTime = millis();
      
      //Serial.print("Button pressed up ");
      
      // 调用回调函数处理按钮按下事件
      if (buttonPressCallback != nullptr) {
        buttonPressCallback();
      }
    }
  }
  
  lastButtonState = currentState;
}

// Getter函数，提供对内部状态的只读访问
bool isButtonPressed() {
  return buttonPressed;
}

unsigned long getLastButtonPressTime() {
  return lastButtonPressTime;
}