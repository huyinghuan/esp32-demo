#include "esp32_power_mgr.h"
#include "../config.h"
#include "../devices/led.h"

// 电源状态变量
unsigned long esp32LastActivity = 0;
esp32_power_mode_t esp32CurrentPowerMode = ESP32_ACTIVE;
bool esp32PowerSaveEnabled = true;

// 配置常量
const unsigned long ESP32_DEEP_SLEEP_THRESHOLD = 600000;    // 10分钟无活动进入深度睡眠
const unsigned long ESP32_LIGHT_SLEEP_THRESHOLD = 60000;    // 1分钟无活动进入轻度睡眠
const uint64_t ESP32_DEFAULT_SLEEP_TIME = 30 * 1000000;     // 默认睡眠30秒

void initESP32PowerManager() {
  Serial.println("初始化ESP32电源管理器...");
  
  esp32LastActivity = millis();
  esp32CurrentPowerMode = ESP32_ACTIVE;
  
  // 设置CPU频率为较低值以节省功耗
  if (esp32PowerSaveEnabled) {
    setESP32CPUFrequency(80); // 设置为80MHz，默认是240MHz
  }
  
  Serial.println("ESP32电源管理器初始化完成");
}

void enterESP32DeepSleep(uint64_t sleepTimeUs) {
  Serial.println("准备进入深度睡眠模式...");
  Serial.print("睡眠时间: ");
  Serial.print(sleepTimeUs / 1000000);
  Serial.println(" 秒");
  
  // 关闭所有LED
  setLED(ledPin, false);
  
  // 配置唤醒源
  setupESP32WakeupTimer(sleepTimeUs);
  
  Serial.println("进入深度睡眠...");
  Serial.flush(); // 确保串口输出完成
  
  esp32CurrentPowerMode = ESP32_DEEP_SLEEP;
  esp_deep_sleep_start();
}

void enterESP32LightSleep(uint64_t sleepTimeUs) {
  Serial.println("进入轻度睡眠模式...");
  
  // 配置唤醒源
  setupESP32WakeupTimer(sleepTimeUs);
  
  esp32CurrentPowerMode = ESP32_LIGHT_SLEEP;
  esp_light_sleep_start();
  
  Serial.println("从轻度睡眠唤醒");
  esp32CurrentPowerMode = ESP32_ACTIVE;
  esp32LastActivity = millis();
}

void setupESP32WakeupTimer(uint64_t sleepTimeUs) {
  esp_sleep_enable_timer_wakeup(sleepTimeUs);
}

void printESP32WakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("唤醒原因: 外部信号 RTC_IO");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("唤醒原因: 外部信号 RTC_CNTL");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("唤醒原因: 定时器");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("唤醒原因: 触摸板");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("唤醒原因: ULP程序");
      break;
    default:
      Serial.println("唤醒原因: 首次启动");
      break;
  }
}

void setESP32CPUFrequency(uint32_t freq) {
  setCpuFrequencyMhz(freq);
  Serial.print("CPU频率设置为: ");
  Serial.print(freq);
  Serial.println(" MHz");
}

void enableESP32CPUPowerSave() {
  setESP32CPUFrequency(80); // 降低到80MHz
  Serial.println("CPU节能模式: 已启用");
}

void disableESP32CPUPowerSave() {
  setESP32CPUFrequency(240); // 恢复到240MHz
  Serial.println("CPU节能模式: 已禁用");
}

void smartESP32PowerManagement() {
  if (!esp32PowerSaveEnabled) return;
  
  unsigned long idleTime = getESP32IdleTime();
  
  // 根据空闲时间决定电源管理策略
  if (shouldEnterESP32DeepSleep()) {
    Serial.println("检测到长时间空闲，准备进入深度睡眠");
    enterESP32DeepSleep(ESP32_DEFAULT_SLEEP_TIME);
  } else if (idleTime > ESP32_LIGHT_SLEEP_THRESHOLD && esp32CurrentPowerMode == ESP32_ACTIVE) {
    Serial.println("检测到短期空闲，进入轻度睡眠");
    enterESP32LightSleep(5 * 1000000); // 5秒轻度睡眠
  }
}

bool shouldEnterESP32DeepSleep() {
  unsigned long idleTime = getESP32IdleTime();
  
  // 只有在以下条件都满足时才进入深度睡眠：
  // 1. 空闲时间超过阈值
  // 2. 当前不在处理重要任务
  // 3. WiFi和MQTT连接稳定或不需要
  
  return (idleTime > ESP32_DEEP_SLEEP_THRESHOLD) && 
         (esp32CurrentPowerMode == ESP32_ACTIVE);
}

unsigned long getESP32IdleTime() {
  return millis() - esp32LastActivity;
}

// 更新活动时间的函数，应该在有用户交互或重要事件时调用
void updateESP32Activity() {
  esp32LastActivity = millis();
  if (esp32CurrentPowerMode != ESP32_ACTIVE) {
    esp32CurrentPowerMode = ESP32_ACTIVE;
    Serial.println("系统唤醒，恢复活跃模式");
  }
}
