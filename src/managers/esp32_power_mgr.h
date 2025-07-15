#ifndef ESP32_POWER_MGR_H
#define ESP32_POWER_MGR_H

#include <Arduino.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"

// ESP32电源管理模式
typedef enum {
  ESP32_ACTIVE,         // 活跃模式
  ESP32_LIGHT_SLEEP,    // 轻度睡眠
  ESP32_DEEP_SLEEP,     // 深度睡眠
  ESP32_HIBERNATION     // 休眠模式
} esp32_power_mode_t;

// 电源管理函数
void initESP32PowerManager();
void enterESP32DeepSleep(uint64_t sleepTimeUs);
void enterESP32LightSleep(uint64_t sleepTimeUs);
void setupESP32WakeupTimer(uint64_t sleepTimeUs);
void printESP32WakeupReason();

// CPU频率管理
void setESP32CPUFrequency(uint32_t freq);
void enableESP32CPUPowerSave();
void disableESP32CPUPowerSave();

// 智能电源管理
void smartESP32PowerManagement();
bool shouldEnterESP32DeepSleep();
unsigned long getESP32IdleTime();
void updateESP32Activity();

// 电源状态变量
extern unsigned long esp32LastActivity;
extern esp32_power_mode_t esp32CurrentPowerMode;
extern bool esp32PowerSaveEnabled;

// 配置常量
extern const unsigned long ESP32_DEEP_SLEEP_THRESHOLD;
extern const unsigned long ESP32_LIGHT_SLEEP_THRESHOLD;
extern const uint64_t ESP32_DEFAULT_SLEEP_TIME;

#endif
