#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// MQTT管理函数
void initMQTT();
void connectToMQTT();
void checkMQTTConnection();
void handleMQTTLoop();
bool isMQTTConnected();
// 发布消息到指定主题
// @Return true if message is published successfully, otherwise false
bool publishMessage(const char* topic, const char* message);
void publishStatus(const char* status);

// MQTT回调函数
void mqttCallback(char* topic, byte* payload, unsigned int length);
void handleCommand(String command);

// MQTT客户端
extern WiFiClient espClient;
extern PubSubClient client;

// MQTT状态变量
extern unsigned long lastMqttCheck;

#endif
