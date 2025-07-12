#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// WiFi管理函数
void initWiFi();
void connectToWiFi();
void checkWiFiConnection();
bool isWiFiConnected();

// WiFi状态变量
extern unsigned long lastWiFiCheck;

#endif
