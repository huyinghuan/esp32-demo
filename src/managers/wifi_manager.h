#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

#define POWER_SAVE_MODE WIFI_POWER_SAVE_ENABLED
// WiFi功耗模式枚举
typedef enum {
  WIFI_POWER_FULL,      // 全功率模式
  WIFI_POWER_MODEM,     // 调制解调器睡眠模式
  WIFI_POWER_LIGHT,     // 轻度睡眠模式
  WIFI_POWER_OFF        // 完全关闭WiFi
} wifi_power_mode_t;

typedef enum {
  WIFI_DISCONNECTED = 0,
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_FAILED
} WiFiStatus;

typedef void (*WiFiStatusHandler)(WiFiStatus status);

// WiFi管理函数
void initWiFi(bool enableSaveMode, WiFiStatusHandler statusHandler = nullptr);
void connectToWiFi();
void checkWiFiConnection();
bool isWiFiConnected();
bool isInCooldown(); // 检查是否在重试冷却期
void resetWiFiRetryState(); // 重置WiFi重试状态（强制允许重新尝试）

// WiFi功耗优化函数
void setWiFiPowerMode(wifi_power_mode_t mode);
void enableWiFiPowerSave();
void disableWiFiPowerSave();
void wifiSleep();
void wifiWakeup();
int getWifiSignal(); // 获取WiFi信号强度等级 (0-4: 0=未连接, 1=很弱, 2=弱, 3=中等, 4=强)
String getIP(); // 获取WiFi状态信息

// WiFi状态变量
extern bool wifiPowerSaveEnabled;
extern wifi_power_mode_t currentWiFiPowerMode;

#endif
