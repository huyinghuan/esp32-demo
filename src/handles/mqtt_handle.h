#ifndef MQTT_HANDLE_H
#define MQTT_HANDLE_H
#include <Arduino.h>
// MQTT回调函数
void mqttCallback(char* topic, byte* payload, unsigned int length);
void handleCommand(String command);
bool isMQTTIdle();
#endif
