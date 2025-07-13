#include "button.h"
#include "led.h"
#include "screen.h"  // 添加screen头文件
#include "../config.h"
#include "../managers/mqtt_manager.h"
#include "../managers/device_id_manager.h"
#include "../messages/messages.h"

// 按钮状态变量（模块内部私有）
static bool buttonPressed = false;
static int lastButtonState = HIGH;
static unsigned long lastDebounceTime = 0;
static unsigned long lastButtonPressTime = 0;

// 表情状态变量
static int currentEmojiType = 0; // 0=笑脸, 1=平静脸

void initButton() {
  pinMode(buttonPin, INPUT_PULLUP);
  buttonPressed = false;
  lastButtonState = HIGH;
  lastDebounceTime = 0;
  lastButtonPressTime = 0;
  
  Serial.println("按钮模块初始化完成");
}

void checkButton() {
  int currentState = digitalRead(buttonPin);
  
  // 检测按钮从高电平变为低电平（按下）
  if (lastButtonState == HIGH && currentState == LOW) {
    buttonPressed = true;
    lastDebounceTime = millis();
    Serial.println("Button pressed down");
  }
  
  // 检测按钮从低电平变为高电平（松开）
  else if (lastButtonState == LOW && currentState == HIGH && buttonPressed) {
    // 防抖处理
    if (millis() - lastDebounceTime > debounceDelay) {
      buttonPressed = false;
      lastButtonPressTime = millis();
      
      Serial.print("Button pressed up ");
      
      // 切换表情显示
      switchToNextEmoji();
      
      // 发送MQTT消息
      sendButtonPressMessage();
      
      // LED闪烁表示按钮按下
      blinkLED(ledPin, 1, 100);
    }
  }
  
  lastButtonState = currentState;
}

void sendButtonPressMessage() {
  if (isMQTTConnected()) {
    // 使用统一的消息结构
    MQTTMessage message = createButtonMessage("pressed");
    String jsonMessage = message.toJSON();
    
    publishMessage(pub_topic_sensor.c_str(), jsonMessage.c_str());
    // Serial.println("MQTT消息发送成功: " + jsonMessage);
  } else {
    Serial.println("MQTT未连接，无法发送消息");
  }
}

// Getter函数，提供对内部状态的只读访问
bool isButtonPressed() {
  return buttonPressed;
}

unsigned long getLastButtonPressTime() {
  return lastButtonPressTime;
}

// 表情状态管理函数
int getCurrentEmojiType() {
  return currentEmojiType;
}

void switchToNextEmoji() {
  // 切换表情类型 (0 <-> 1)
  currentEmojiType = (currentEmojiType == 0) ? 1 : 0;
  
  // 显示新的表情
  drawEmojiByType(currentEmojiType);
  
  // 打印当前状态
  if (currentEmojiType == 0) {
    Serial.println("切换到笑脸表情 😊");
  } else {
    Serial.println("切换到平静表情 😐");
  }
}
