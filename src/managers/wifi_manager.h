#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// WiFi功耗模式枚举
typedef enum {
  WIFI_POWER_FULL,      // 全功率模式
  WIFI_POWER_MODEM,     // 调制解调器睡眠模式
  WIFI_POWER_LIGHT,     // 轻度睡眠模式
  WIFI_POWER_OFF        // 完全关闭WiFi
} wifi_power_mode_t;

// WiFi管理函数
void initWiFi();
void connectToWiFi();
void checkWiFiConnection();
bool isWiFiConnected();

// WiFi功耗优化函数
void setWiFiPowerMode(wifi_power_mode_t mode);
void enableWiFiPowerSave();
void disableWiFiPowerSave();
void wifiSleep();
void wifiWakeup();

// 智能WiFi管理
void smartWiFiManagement();
void scheduleWiFiWakeup(unsigned long intervalMs);

// WiFi状态变量
extern unsigned long lastWiFiCheck;
extern unsigned long lastWiFiActivity;
extern bool wifiPowerSaveEnabled;
extern wifi_power_mode_t currentWiFiPowerMode;

#endif
