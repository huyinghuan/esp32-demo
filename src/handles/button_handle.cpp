
#include <Arduino.h>
#include "button_handle.h"
#include <devices/led.h>
// 按钮按下事件处理函数
void onButtonPressed() {
    Serial.println("处理按钮按下事件");
    blinkLED(ledPin, 1, 100); // LED闪烁表示按钮按下
    // 使用统一的消息结构
    MQTTMessage message = createButtonMessage("pressed");
    String jsonMessage = message.toJSON();

    publishMessage(pub_topic_sensor.c_str(), jsonMessage.c_str());
    Serial.println("MQTT消息发送成功: " + jsonMessage);
}