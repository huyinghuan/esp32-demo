#ifndef ESP32_POWER_MGR_H
#define ESP32_POWER_MGR_H

#include <Arduino.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "driver/gpio.h"
#include "driver/uart.h"

// ESP32电源管理模式
typedef enum {
  ESP32_ACTIVE,         // 活跃模式
  ESP32_LIGHT_SLEEP,    // 轻度睡眠
  ESP32_DEEP_SLEEP,     // 深度睡眠
} esp32_power_mode_t;

typedef void (*RunOnLightSleep)(); // 轻度睡眠函数类型
typedef void (*RunOnDeepSleep)(); // 深度睡眠函数类型

void setSleepCallback(RunOnDeepSleep cb1, RunOnLightSleep cb2);

// 电源管理函数
void initESP32PowerManager(RunOnDeepSleep cb1, RunOnLightSleep cb2);
void enterDeepSleep(uint64_t sleepTimeUs);
void enterLightSleep(uint64_t sleepTimeUs);
// void enterESP32LightSleepWithWakeup(uint64_t sleepTimeUs, bool enableGPIO, gpio_num_t gpio_pin, bool enableUART, bool enableWiFi);
// void setupESP32WakeupTimer(uint64_t sleepTimeUs);
// void setupESP32GPIOWakeup(gpio_num_t gpio_pin, int level);
// void setupESP32UARTWakeup(uart_port_t uart_num, int threshold);
// void setupESP32WiFiWakeup();
// void setupESP32BluetoothWakeup();
void printESP32WakeupReason();



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
