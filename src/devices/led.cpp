#include "led.h"

// LED状态跟踪结构体
struct LEDState {
  int pin;
  bool currentState;
  bool isBlinking;
  unsigned long lastBlinkTime;
  int blinkCount;
  int totalBlinks;
  int blinkDelay;
};

// 支持最多8个LED的状态跟踪
static LEDState ledStates[8];
static int ledCount = 0;

// 查找或创建LED状态
static LEDState* findOrCreateLEDState(int pin) {
  // 查找现有的LED
  for (int i = 0; i < ledCount; i++) {
    if (ledStates[i].pin == pin) {
      return &ledStates[i];
    }
  }
  
  // 创建新的LED状态（如果还有空间）
  if (ledCount < 8) {
    ledStates[ledCount].pin = pin;
    ledStates[ledCount].currentState = false;
    ledStates[ledCount].isBlinking = false;
    ledStates[ledCount].lastBlinkTime = 0;
    ledStates[ledCount].blinkCount = 0;
    ledStates[ledCount].totalBlinks = 0;
    ledStates[ledCount].blinkDelay = 0;
    return &ledStates[ledCount++];
  }
  
  return nullptr; // 超出最大LED数量
}

void initLED(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); // ESP32上通常HIGH为关闭
  
  // 初始化LED状态
  LEDState* state = findOrCreateLEDState(pin);
  if (state) {
    state->currentState = false; // false = 关闭
  }
  
  Serial.println("LED初始化完成 - Pin: " + String(pin));
}

void setLED(int pin, bool state) {
  // ESP32上LED逻辑：LOW=亮，HIGH=灭
  digitalWrite(pin, state ? LOW : HIGH);
  
  LEDState* ledState = findOrCreateLEDState(pin);
  if (ledState) {
    ledState->currentState = state;
    ledState->isBlinking = false; // 停止异步闪烁
  }
}

void toggleLED(int pin) {
  LEDState* ledState = findOrCreateLEDState(pin);
  if (ledState) {
    setLED(pin, !ledState->currentState);
  }
}

void blinkLED(int pin, int times, int delayMs) {
  // 阻塞式闪烁
  for (int i = 0; i < times; i++) {
    setLED(pin, true);  // 亮
    delay(delayMs);
    setLED(pin, false); // 灭
    if (i < times - 1) { // 最后一次不延迟
      delay(delayMs);
    }
  }
}

void blinkLEDAsync(int pin, int times, int delayMs) {
  // 非阻塞式闪烁设置
  LEDState* ledState = findOrCreateLEDState(pin);
  if (ledState) {
    ledState->isBlinking = true;
    ledState->blinkCount = 0;
    ledState->totalBlinks = times * 2; // 亮+灭 = 2次状态变化
    ledState->blinkDelay = delayMs;
    ledState->lastBlinkTime = millis();
    setLED(pin, true); // 开始时点亮
  }
}

bool getLEDState(int pin) {
  LEDState* ledState = findOrCreateLEDState(pin);
  return ledState ? ledState->currentState : false;
}

// 处理异步闪烁的更新函数（需要在主循环中调用）
void updateLEDs() {
  unsigned long currentTime = millis();
  
  for (int i = 0; i < ledCount; i++) {
    LEDState* led = &ledStates[i];
    
    if (led->isBlinking && (currentTime - led->lastBlinkTime >= led->blinkDelay)) {
      led->blinkCount++;
      
      if (led->blinkCount >= led->totalBlinks) {
        // 闪烁完成
        led->isBlinking = false;
        setLED(led->pin, false); // 确保最后是关闭状态
      } else {
        // 切换LED状态
        toggleLED(led->pin);
        led->lastBlinkTime = currentTime;
      }
    }
  }
}
