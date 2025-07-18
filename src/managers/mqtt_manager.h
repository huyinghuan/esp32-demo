#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

typedef void (*MqttCallback)(char* topic, byte* payload, unsigned int length);
// MQTT管理函数
void initMQTT(MqttCallback callback);
void connectToMQTT();
void checkMQTTConnection();
void handleMQTTLoop();
bool isMQTTConnected();
// 发布消息到指定主题
// @Return true if message is published successfully, otherwise false
bool publishMessage(const char* topic, const char* message);
void publishStatus(const char* status);


// MQTT客户端
extern WiFiClient espClient;
extern PubSubClient client;

// MQTT状态变量
extern unsigned long lastMqttCheck;

#endif
