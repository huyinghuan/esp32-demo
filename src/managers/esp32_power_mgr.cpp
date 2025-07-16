#include "esp32_power_mgr.h"
// #include "../config.h"
// #include "../devices/led.h"
// #include <devices/screen.h>
// #include "driver/gpio.h"
// #include "driver/uart.h"

// 电源状态变量
unsigned long esp32LastActivity = 0;
esp32_power_mode_t esp32CurrentPowerMode = ESP32_ACTIVE;

// 配置常量
const unsigned long ESP32_DEEP_SLEEP_THRESHOLD = 10*60*1000;    // 10分钟无活动进入深度睡眠
const unsigned long ESP32_LIGHT_SLEEP_THRESHOLD = 1*60*1000;    // 1分钟无活动进入轻度睡眠
const uint64_t ESP32_DEFAULT_SLEEP_TIME = 30 * 1000000;     // 默认睡眠30秒
const uint64_t ESP32_LIGHT_SLEEP_TIME = 5 * 1000000; // 轻度睡眠5秒


// 按钮按下事件回调函数指针
static RunOnDeepSleep runOnDeepSleepCallback = nullptr;
static RunOnLightSleep runOnLightSleepCallback = nullptr;

void setSleepCallback(RunOnDeepSleep cb1, RunOnLightSleep cb2) {
    runOnDeepSleepCallback = cb1;
    runOnLightSleepCallback = cb2;
}

//初始化ESP32电源管理器
void initESP32PowerManager(RunOnDeepSleep cb1, RunOnLightSleep cb2) {
    runOnDeepSleepCallback = cb1;
    runOnLightSleepCallback = cb2;
    esp32LastActivity = millis();
    esp32CurrentPowerMode = ESP32_ACTIVE;
}

void enterDeepSleep(uint64_t sleepTimeUs) {
    if (sleepTimeUs == 0) {
        sleepTimeUs = ESP32_DEFAULT_SLEEP_TIME; // 默认30秒
    }
    Serial.println("准备进入深度睡眠模式...");
    Serial.print("睡眠时间: ");
    Serial.print(sleepTimeUs / 1000000);
    Serial.println(" 秒");
    

    if (runOnDeepSleepCallback != nullptr) {
        runOnDeepSleepCallback(); // 调用深度睡眠回调函数
    }

    // 关闭所有LED
    // setLED(ledPin, false);
    // 配置唤醒源
    // setupESP32WakeupTimer(sleepTimeUs);

    Serial.flush(); // 确保串口输出完成
    
    esp32CurrentPowerMode = ESP32_DEEP_SLEEP;
    esp_deep_sleep_start();
}

void enterLightSleep(uint64_t sleepTimeUs) {
    Serial.println("进入轻度睡眠模式...");
    if(sleepTimeUs == 0) {
        sleepTimeUs = ESP32_LIGHT_SLEEP_THRESHOLD; // 默认30秒
    }
    // 配置唤醒源
    //setupESP32WakeupTimer(sleepTimeUs);

    if (runOnLightSleepCallback != nullptr) {
        runOnLightSleepCallback(); // 调用轻度睡眠回调函数
    }

    esp32CurrentPowerMode = ESP32_LIGHT_SLEEP;
    esp_light_sleep_start();
    
    updateESP32Activity(); // 更新活动时间
}

// 带多种唤醒源的轻度睡眠函数
// void enterESP32LightSleepWithWakeup(uint64_t sleepTimeUs, bool enableGPIO, gpio_num_t gpio_pin, bool enableUART, bool enableWiFi) {
//   Serial.println("进入轻度睡眠模式 (多唤醒源)...");
  
//   // 清除之前的唤醒源配置
//   esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  
//   // 配置定时器唤醒 (如果指定时间)
//   if (sleepTimeUs > 0) {
//     setupESP32WakeupTimer(sleepTimeUs);
//   }
  
//   // 配置GPIO唤醒
//   if (enableGPIO) {
//     setupESP32GPIOWakeup(gpio_pin, 0); // 低电平触发
//   }
  
//   // 配置UART唤醒
//   if (enableUART) {
//     setupESP32UARTWakeup(UART_NUM_0, 3); // 串口0，3字节阈值
//   }
  
//   // 配置WiFi唤醒
//   if (enableWiFi) {
//     setupESP32WiFiWakeup();
//   }
  
//   esp32CurrentPowerMode = ESP32_LIGHT_SLEEP;
//   esp_light_sleep_start();
  
//   // 睡眠结束后打印唤醒原因
//   printESP32WakeupReason();
//   updateESP32Activity(); // 更新活动时间
// }

// void setupESP32WakeupTimer(uint64_t sleepTimeUs) {
//   esp_sleep_enable_timer_wakeup(sleepTimeUs);
// }

// 配置GPIO唤醒源 (适用于轻度睡眠)
// void setupESP32GPIOWakeup(gpio_num_t gpio_pin, int level) {
//   // 配置GPIO为输入并启用内部上拉/下拉
//   gpio_config_t config = {
//     .pin_bit_mask = (1ULL << gpio_pin),
//     .mode = GPIO_MODE_INPUT,
//     .pull_up_en = (level == 0) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
//     .pull_down_en = (level == 1) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
//     .intr_type = GPIO_INTR_DISABLE
//   };
//   gpio_config(&config);
  
//   // 启用GPIO唤醒
//   esp_sleep_enable_gpio_wakeup();
//   Serial.print("GPIO唤醒源已配置: Pin ");
//   Serial.print(gpio_pin);
//   Serial.print(", 触发电平: ");
//   Serial.println(level);
// }

// // 配置UART唤醒源
// void setupESP32UARTWakeup(uart_port_t uart_num, int threshold) {
//   esp_sleep_enable_uart_wakeup(uart_num);
//   Serial.print("UART唤醒源已配置: UART");
//   Serial.print(uart_num);
//   Serial.print(", 阈值: ");
//   Serial.println(threshold);
// }

// // 配置WiFi唤醒源 (仅轻度睡眠)
// void setupESP32WiFiWakeup() {
//   esp_sleep_enable_wifi_wakeup();
//   Serial.println("WiFi唤醒源已配置");
// }

// 配置蓝牙唤醒源 (仅轻度睡眠)
// void setupESP32BluetoothWakeup() {
//   // 注意：蓝牙唤醒需要在初始化蓝牙后配置
//   // 在轻度睡眠模式下，蓝牙可以保持活跃状态来唤醒系统
//   Serial.println("蓝牙唤醒源已配置 (需要蓝牙模块初始化)");
// }

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


void smartESP32PowerManagement() {
  
  unsigned long idleTime = getESP32IdleTime();
  // 根据空闲时间决定电源管理策略
  if (shouldEnterESP32DeepSleep()) {
    Serial.println("检测到长时间空闲，准备进入深度睡眠");
    enterDeepSleep(ESP32_DEFAULT_SLEEP_TIME);
  } else if (idleTime > ESP32_LIGHT_SLEEP_THRESHOLD && esp32CurrentPowerMode == ESP32_ACTIVE) {
    Serial.println("检测到短期空闲，进入轻度睡眠");
    enterLightSleep(ESP32_LIGHT_SLEEP_TIME); // 5秒轻度睡眠
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
  }
}

// 测试无唤醒源的轻度睡眠行为
// void testLightSleepWithoutWakeupSource() {
//   Serial.println("\n=== 测试无唤醒源的轻度睡眠 ===");
  
//   // 先清除所有唤醒源
//   esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  
//   Serial.println("警告: 即将进入无唤醒源的轻度睡眠");
//   Serial.println("理论上只能通过以下方式唤醒:");
//   Serial.println("1. 硬件复位");
//   Serial.println("2. 外部中断 (如果GPIO配置了中断)");
//   Serial.println("3. 系统内部事件 (如WiFi活动)");
  
//   Serial.println("进入轻度睡眠... (可能需要手动复位来唤醒)");
//   Serial.flush();
  
//   unsigned long beforeSleep = millis();
//   esp_light_sleep_start();
//   unsigned long afterSleep = millis();
  
//   Serial.println("已从轻度睡眠唤醒!");
//   Serial.print("睡眠时长: ");
//   Serial.print(afterSleep - beforeSleep);
//   Serial.println(" ms");
  
//   // 打印唤醒原因
//   printESP32WakeupReason();
// }


// 使用示例函数
// void demonstrateWakeupSources() {
//   Serial.println("\n=== ESP32轻度睡眠唤醒源示例 ===");
  
//   // 示例1: 仅GPIO唤醒 (适用于按钮中断)
//   Serial.println("示例1: 使用GPIO唤醒 (按钮按下)");
//   Serial.println("配置: GPIO0, 低电平触发");
//   // enterESP32LightSleepWithWakeup(0, true, GPIO_NUM_0, false, false);
  
//   // 示例2: 仅UART唤醒 (适用于串口通信)
//   Serial.println("示例2: 使用UART唤醒 (串口数据)");
//   Serial.println("配置: UART0, 3字节阈值");
//   // enterESP32LightSleepWithWakeup(0, false, GPIO_NUM_0, true, false);
  
//   // 示例3: WiFi + 定时器唤醒 (适用于IoT应用)
//   Serial.println("示例3: 使用WiFi + 定时器唤醒");
//   Serial.println("配置: 10秒定时器 + WiFi数据包");
//   // enterESP32LightSleepWithWakeup(10 * 1000000, false, GPIO_NUM_0, false, true);
  
//   // 示例4: 多种唤醒源组合
//   Serial.println("示例4: 多种唤醒源组合");
//   Serial.println("配置: 30秒定时器 + GPIO0 + UART0 + WiFi");
//   // enterESP32LightSleepWithWakeup(30 * 1000000, true, GPIO_NUM_0, true, true);
// }


