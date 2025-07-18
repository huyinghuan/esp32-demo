#include <Arduino.h>
#include "config.h"
#include "managers/managers.h"
#include "messages/messages.h"
#include "devices/devices.h"
#include "handles/handle.h"
#include "output/status_manager.h"

// 基于设备ID和主题
// String pub_topic_button_press;
String pub_topic_status; // 状态主题
String pub_topic_sensor; // 传感器主题
String sub_topic_command;

const int buttonPin = 14;
const int ledPin = 2;

// 初始化设备主题
void initDeviceTopics() {
  int deviceID = getDeviceID();
  // pub_topic_button_press = "esp32/" + String(deviceID) + "/button/press";
  pub_topic_status = "esp32/" + String(deviceID) + "/status";
  pub_topic_sensor = "esp32/" + String(deviceID) + "/sensor";
  sub_topic_command = "esp32/" + String(deviceID) + "/command";
  
  DEBUG_PRINTLN("设备主题初始化完成:");
  //DEBUG_PRINTLN("  按钮主题: " + pub_topic_button_press);
  DEBUG_PRINTLN("  状态主题: " + pub_topic_status);
  DEBUG_PRINTLN("  传感器主题: " + pub_topic_sensor);
  DEBUG_PRINTLN("  命令主题: " + sub_topic_command);
}

void setup() {
  // 初始化串口
  Serial.begin(115200);
  DEBUG_PRINTLN("ESP32 MQTT Button Client Starting...");
  // 检查唤醒原因（如果从睡眠中唤醒）
  // #if POWER_MANAGEMENT_ENABLED
  // printESP32WakeupReason();
  // #endif
  
  // 初始化电源管理器
  #if POWER_MANAGEMENT_ENABLED
  // initESP32PowerManager(); // 默认进入省电模式
  #endif
  
  // 初始化设备ID
  initDeviceID();
  // 初始化设备主题
  initDeviceTopics();
  
  // 配置GPIO
  initLED(ledPin);
  initScreen();
  
  initWiFi(WIFI_POWER_SAVE_ENABLED, handleWifiStatusChange); // 初始化WiFi
  
  initMQTT(mqttCallback); // 初始化MQTT

  // 根据配置决定是否初始化蓝牙（默认禁用以节省功耗）
  // initBluetooth();
  
  // Checked OK
  //initButton(buttonPin, onButtonPressed); // 初始化按钮  设置按钮按下回调函数
  
  // 显示设备信息
  DEBUG_PRINTLN("\n=== 设备信息 ===");
  DEBUG_PRINTLN("设备ID: " + getDeviceIDString());
  DEBUG_PRINTLN("===============\n");
}

void loop() {
  // 智能电源管理
  // #if SMART_POWER_SAVE_MODE
  // smartESP32PowerManagement();
  // #endif
  
  // 检查各个连接状态
  checkWiFiConnection();

  printStatusToScreen(); // 打印状态到屏幕

  // 检查蓝牙连接状态
  // checkBluetoothConnection();
  
  // 处理MQTT消息
  handleMQTTLoop();
  
  // 检查按钮状态
  checkButton();
  
  // 发送心跳
  checkHeartbeat();
  
  // 处理串口命令
  handleSerialCommand();
  
  // 更新LED状态（处理异步闪烁）
  updateLEDs();
  
  delay(10);
}

