#include "wifi_manager.h"
#include "../config.h"
#include "../devices/led.h"

// 只有在启用WiFi时才编译此文件
#if WIFI_ENABLED

// WiFi状态变量
unsigned long lastWiFiCheck = 0;
unsigned long lastWiFiActivity = 0;
bool wifiPowerSaveEnabled = POWER_SAVE_MODE;
wifi_power_mode_t currentWiFiPowerMode = WIFI_POWER_FULL;

// WiFi空闲超时时间（毫秒）
const unsigned long WIFI_IDLE_TIMEOUT = 300000; // 5分钟
const unsigned long WIFI_SLEEP_DURATION = 60000; // 1分钟睡眠

void initWiFi() {
  Serial.println("初始化WiFi...");
  WiFi.mode(WIFI_STA);
  connectToWiFi();
}

void connectToWiFi() {
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  
  // 设置为全功率模式进行连接
  setWiFiPowerMode(WIFI_POWER_FULL);
  
  WiFi.begin(ssid, password);
  
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    delay(500);
    Serial.print(".");
    retryCount++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi连接成功!");
    Serial.print("IP地址: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
    
    setLED(ledPin, true);  // WiFi连接成功，点亮LED
    lastWiFiActivity = millis();
    
    // 连接成功后切换到节能模式
    if (wifiPowerSaveEnabled) {
      setWiFiPowerMode(WIFI_POWER_MODEM);
    }
  } else {
    Serial.println();
    Serial.println("WiFi连接失败!");
    setLED(ledPin, false);
  }
}

void checkWiFiConnection() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastWiFiCheck >= wifiCheckInterval) {
    lastWiFiCheck = currentTime;
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi断开，尝试重连...");
      setLED(ledPin, false);
      connectToWiFi();
    } else {
      // 更新最后活动时间
      lastWiFiActivity = currentTime;
    }
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// 设置WiFi功耗模式
void setWiFiPowerMode(wifi_power_mode_t mode) {
  currentWiFiPowerMode = mode;
  
  switch(mode) {
    case WIFI_POWER_FULL:
      WiFi.setSleep(false);
      Serial.println("WiFi: 全功率模式");
      break;
      
    case WIFI_POWER_MODEM:
      WiFi.setSleep(WIFI_PS_MIN_MODEM);
      Serial.println("WiFi: 调制解调器睡眠模式");
      break;
      
    case WIFI_POWER_LIGHT:
      WiFi.setSleep(WIFI_PS_MAX_MODEM);
      Serial.println("WiFi: 轻度睡眠模式");
      break;
      
    case WIFI_POWER_OFF:
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      Serial.println("WiFi: 完全关闭");
      break;
  }
}

// 启用WiFi节能模式
void enableWiFiPowerSave() {
  wifiPowerSaveEnabled = true;
  Serial.println("WiFi节能模式: 已启用");
}

// 禁用WiFi节能模式
void disableWiFiPowerSave() {
  wifiPowerSaveEnabled = false;
  setWiFiPowerMode(WIFI_POWER_FULL);
  Serial.println("WiFi节能模式: 已禁用");
}

// WiFi睡眠
void wifiSleep() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi进入睡眠模式");
    setWiFiPowerMode(WIFI_POWER_OFF);
  }
}

// WiFi唤醒
void wifiWakeup() {
  if (currentWiFiPowerMode == WIFI_POWER_OFF) {
    Serial.println("WiFi从睡眠模式唤醒");
    WiFi.mode(WIFI_STA);
    connectToWiFi();
  }
}

#endif