#include "wifi_manager.h"
#include "../secrets.h"
#include "../config.h"
// 只有在启用WiFi时才编译此文件  选择性编译降低flash占用和功耗
#if WIFI_ENABLED

// WiFi状态变量
unsigned long lastWiFiCheck = 0;
unsigned long lastWiFiActivity = 0;
unsigned long lastFailedConnectTime = 0; // 上次连接失败的时间
int consecutiveFailCount = 0; // 连续失败次数
bool wifiPowerSaveEnabled = false;
wifi_power_mode_t currentWiFiPowerMode = WIFI_POWER_FULL;

// WiFi空闲超时时间（毫秒）
const unsigned long WIFI_IDLE_TIMEOUT = 300000; // 5分钟
const unsigned long WIFI_SLEEP_DURATION = 60000; // 1分钟睡眠
const unsigned long WIFI_RETRY_COOLDOWN = 60000; // 60秒重试冷却期
const int MAX_CONSECUTIVE_FAILS = 3; // 最大连续失败次数

const char* _ssid = WIFI_SSID;
const char* _password = WIFI_PASSWORD;

const bool auto_connect = WIFI_AUTO_CONNECT; // 是否自动连接WiFi

const unsigned long wifiCheckInterval = 5000;

WiFiStatusHandler _statusChangeHandler = nullptr;
void defaultWiFiStatusHandler(WiFiStatus status) {}

void initWiFi(bool enableSaveMode, WiFiStatusHandler statusHandler) {
  DEBUG_PRINTLN("初始化WiFi...");
  wifiPowerSaveEnabled = enableSaveMode;
  WiFi.mode(WIFI_STA);
  if(statusHandler != nullptr) {
    _statusChangeHandler = statusHandler;
  }else{
    _statusChangeHandler = defaultWiFiStatusHandler; // 使用默认处理函数
  }

  _statusChangeHandler(WIFI_CONNECTING); // 设置初始状态为连接中
  connectToWiFi();
}

void connectToWiFi() {
  if(isWiFiConnected()) {
    _statusChangeHandler(WIFI_CONNECTED);
    return;
  }
  
  // 检查是否在冷却期内
  unsigned long currentTime = millis();
  if (consecutiveFailCount >= MAX_CONSECUTIVE_FAILS) {
    if (currentTime - lastFailedConnectTime < WIFI_RETRY_COOLDOWN) { // "WiFi连接冷却期，还需等待 "
      unsigned long remainingTime = (WIFI_RETRY_COOLDOWN - (currentTime - lastFailedConnectTime)) / 1000;
      DEBUG_PRINTLN("WiFi连接冷却期，还需等待 ");
      DEBUG_PRINTLN(remainingTime);
      DEBUG_PRINTLN(" 秒");
      return;
    } else {
      // 冷却期结束，重置失败计数
      DEBUG_PRINTLN("WiFi连接冷却期结束，重置重试计数");
      consecutiveFailCount = 0;
    }
  }
  
  DEBUG_PRINT("正在连接WiFi: ");
  DEBUG_PRINTLN(_ssid);
  
  // 设置为全功率模式进行连接
  setWiFiPowerMode(WIFI_POWER_FULL);
  
  WiFi.begin(_ssid, _password);
  
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 5) {
    delay(500);
    DEBUG_PRINT(".");
    retryCount++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN();
    DEBUG_PRINTLN("WiFi连接成功!");
    DEBUG_PRINT("IP地址: ");
    DEBUG_PRINTLN(WiFi.localIP());
    DEBUG_PRINT("RSSI: ");
    DEBUG_PRINTLN(WiFi.RSSI());
    
    // 连接成功，重置失败计数
    consecutiveFailCount = 0;
    lastFailedConnectTime = 0;
    
    //setLED(ledPin, true);  // WiFi连接成功，点亮LED
    lastWiFiActivity = millis();
    
    _statusChangeHandler(WIFI_CONNECTED); // 调用状态处理函数
    // 连接成功后切换到节能模式
    // if (wifiPowerSaveEnabled) {
    //   setWiFiPowerMode(WIFI_POWER_MODEM);
    // }
  } else {
    DEBUG_PRINTLN("WiFi连接失败!");
    
    // 记录失败信息
    consecutiveFailCount++;
    lastFailedConnectTime = currentTime;
    
    DEBUG_PRINT("连续失败次数: ");
    DEBUG_PRINT(consecutiveFailCount);
    DEBUG_PRINT("/");
    DEBUG_PRINTLN(MAX_CONSECUTIVE_FAILS);
    
    if (consecutiveFailCount >= MAX_CONSECUTIVE_FAILS) {
      DEBUG_PRINT("达到最大失败次数，进入 ");
      DEBUG_PRINT(WIFI_RETRY_COOLDOWN / 1000);
      DEBUG_PRINTLN(" 秒冷却期");
    }
    _statusChangeHandler(WIFI_FAILED); // 调用状态处理函数
  }

}

void checkWiFiConnection() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastWiFiCheck >= wifiCheckInterval) {
    lastWiFiCheck = currentTime;
    
    if (WiFi.status() != WL_CONNECTED) {
      DEBUG_PRINTLN("WiFi断开，尝试重连...");
     // setLED(ledPin, false);

      _statusChangeHandler(WIFI_CONNECTING); // 调用状态处理函数
      if(auto_connect){
        connectToWiFi();
      }
    } else {
      // 更新最后活动时间
      lastWiFiActivity = currentTime;
    }
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// 检查是否在冷却期
bool isInCooldown() {
  if (consecutiveFailCount < MAX_CONSECUTIVE_FAILS) {
    return false;
  }
  
  unsigned long currentTime = millis();
  return (currentTime - lastFailedConnectTime < WIFI_RETRY_COOLDOWN);
}

// 重置WiFi重试状态（强制允许重新尝试连接）
void resetWiFiRetryState() {
  consecutiveFailCount = 0;
  lastFailedConnectTime = 0;
  DEBUG_PRINTLN("WiFi重试状态已重置");
}

// 设置WiFi功耗模式
void setWiFiPowerMode(wifi_power_mode_t mode) {
  currentWiFiPowerMode = mode;
  
  switch(mode) {
    case WIFI_POWER_FULL:
      WiFi.setSleep(false);
      DEBUG_PRINTLN("WiFi: 全功率模式");
      break;
      
    case WIFI_POWER_MODEM:
      WiFi.setSleep(WIFI_PS_MIN_MODEM);
      DEBUG_PRINTLN("WiFi: 调制解调器睡眠模式");
      break;
      
    case WIFI_POWER_LIGHT:
      WiFi.setSleep(WIFI_PS_MAX_MODEM);
      DEBUG_PRINTLN("WiFi: 轻度睡眠模式");
      break;
      
    case WIFI_POWER_OFF:
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      DEBUG_PRINTLN("WiFi: 完全关闭");
      break;
  }
}

// 启用WiFi节能模式
void enableWiFiPowerSave() {
  wifiPowerSaveEnabled = true;
  DEBUG_PRINTLN("WiFi节能模式: 已启用");
}

// 禁用WiFi节能模式
void disableWiFiPowerSave() {
  wifiPowerSaveEnabled = false;
  setWiFiPowerMode(WIFI_POWER_FULL);
  DEBUG_PRINTLN("WiFi节能模式: 已禁用");
}

// WiFi睡眠
void wifiSleep() {
  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN("WiFi进入睡眠模式");
    setWiFiPowerMode(WIFI_POWER_OFF);
  }
}

// WiFi唤醒
void wifiWakeup() {
  if (currentWiFiPowerMode == WIFI_POWER_OFF) {
    DEBUG_PRINTLN("WiFi从睡眠模式唤醒");
    WiFi.mode(WIFI_STA);
    connectToWiFi();
  }
}

// 获取WiFi信号强度等级 (0-4)
// 0: 未连接, 1: 很弱, 2: 弱, 3: 中等, 4: 强
int getWifiSignal() {
  if (WiFi.status() != WL_CONNECTED) {
    return 0; // 未连接
  }
  
  int rssi = WiFi.RSSI();
  
  // 根据RSSI值分级 (dBm)
  if (rssi >= -50) {
    return 4; // 信号很强 (-50 dBm 或更高)
  } else if (rssi >= -60) {
    return 3; // 信号中等 (-50 到 -60 dBm)
  } else if (rssi >= -70) {
    return 2; // 信号较弱 (-60 到 -70 dBm)
  } else if (rssi >= -80) {
    return 1; // 信号很弱 (-70 到 -80 dBm)
  } else {
    return 1; // 信号极弱 (-80 dBm 以下，但仍连接)
  }
}

// 获取WiFi状态信息
String getIP() {
  if (WiFi.status() != WL_CONNECTED) {
    return "";
  }
  return WiFi.localIP().toString();
}

#else
void initWiFi(bool enableSaveMode) {
  DEBUG_PRINTLN("WiFi未启用，跳过初始化");
}
void checkWiFiConnection() {
  DEBUG_PRINTLN("WiFi未启用，跳过WiFi连接检查");
}
// 获取WiFi信号强度等级 (WiFi未启用时返回0)
int getWifiSignal() {
  return 0; // WiFi未启用，返回0 (未连接)
}

bool isInCooldown() {
  return false;
}
void resetWiFiRetryState() {
  // WiFi未启用时无需操作
}
#endif