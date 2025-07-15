#include <Arduino.h>
#include "config.h"
#include "secrets.h"
#include "managers/managers.h"
#include "devices/button.h"
#include "devices/led.h"
#include "messages/messages.h"
#include "devices/screen.h"

// 配置变量定义（从secrets.h获取）
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_pass = MQTT_PASSWORD;

// 基于设备ID和主题
// String pub_topic_button_press;
String pub_topic_status; // 状态主题
String pub_topic_sensor; // 传感器主题
String sub_topic_command;

const int buttonPin = 14;
const int ledPin = 2;

const unsigned long wifiCheckInterval = 5000;
const unsigned long mqttCheckInterval = 1000;
const unsigned long heartbeatInterval = 30000;
const unsigned long debounceDelay = 50;

// 按钮按下事件处理函数
void onButtonPressed() {
  Serial.println("处理按钮按下事件");
  
  #if WIFI_ENABLED && MQTT_ENABLED
  if (isMQTTConnected()) {
    // 使用统一的消息结构
    MQTTMessage message = createButtonMessage("pressed");
    String jsonMessage = message.toJSON();
    
    publishMessage(pub_topic_sensor.c_str(), jsonMessage.c_str());
    Serial.println("MQTT消息发送成功: " + jsonMessage);
  } else {
    Serial.println("MQTT未连接，无法发送消息");
  }
  #else
  Serial.println("MQTT功能未启用");
  #endif
}

// 初始化设备主题
void initDeviceTopics() {
  int deviceID = getDeviceID();
  // pub_topic_button_press = "esp32/" + String(deviceID) + "/button/press";
  pub_topic_status = "esp32/" + String(deviceID) + "/status";
  pub_topic_sensor = "esp32/" + String(deviceID) + "/sensor";
  sub_topic_command = "esp32/" + String(deviceID) + "/command";
  
  Serial.println("设备主题初始化完成:");
  //Serial.println("  按钮主题: " + pub_topic_button_press);
  Serial.println("  状态主题: " + pub_topic_status);
  Serial.println("  传感器主题: " + pub_topic_sensor);
  Serial.println("  命令主题: " + sub_topic_command);
}

void setup() {
  // 初始化串口
  Serial.begin(115200);
  Serial.println("ESP32 MQTT Button Client Starting...");
  
  // 检查唤醒原因（如果从睡眠中唤醒）
  #if POWER_MANAGEMENT_ENABLED
  printESP32WakeupReason();
  #endif
  
  // 初始化电源管理器
  #if POWER_MANAGEMENT_ENABLED
  initESP32PowerManager(); // 默认进入省电模式
  #endif
  
  // 初始化设备ID和主题
  initDeviceID();
  
  // 配置GPIO
  initLED(ledPin);
  initScreen();
  
  // 初始化各个模块 选择性编译降低flash占用和功耗
  #if WIFI_ENABLED
  initWiFi(); // 初始化WiFi
  #endif
  
  #if WIFI_ENABLED  && MQTT_ENABLED
  initDeviceTopics();
  initMQTT(); // 初始化MQTT
  initHeartbeat(); // 初始化心跳
  #endif

  // 根据配置决定是否初始化蓝牙（默认禁用以节省功耗）
  initBluetooth();


  initButton(onButtonPressed); // 初始化按钮  设置按钮按下回调函数
  
  // 显示设备信息
  Serial.println("\n=== 设备信息 ===");
  Serial.println("设备ID: " + getDeviceIDString());
  Serial.println("===============\n");
}

void loop() {
  // 智能电源管理
  #if SMART_POWER_SAVE_MODE
  smartESP32PowerManagement();
  #endif
  
  // 检查各个连接状态
  // checkWiFiConnection();
  // checkMQTTConnection();
  
  // 检查蓝牙连接状态
  // checkBluetoothConnection();
  
  // 处理MQTT消息
  // handleMQTTLoop();
  
  // 检查按钮状态
  checkButton();
  
  // 发送心跳
  // checkHeartbeat();
  
  // 处理串口命令
  handleSerialCommand();
  
  // 更新LED状态（处理异步闪烁）
  updateLEDs();
  
  delay(10);
}

