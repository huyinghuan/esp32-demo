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
  
  // 初始化设备ID和主题
  initDeviceID();
  initDeviceTopics();
  
  // 配置GPIO
  initLED(ledPin);
  initScreen();
  
  // 初始化各个模块
  // 暂时不需要的模块注释
  // initWiFi(); // 初始化WiFi
  // initMQTT(); // 初始化MQTT
  // initHeartbeat(); // 初始化心跳
  // initButton(); // 初始化按钮
  
  // 显示设备信息
  Serial.println("\n=== 设备信息 ===");
  Serial.println("设备ID: " + getDeviceIDString());
  Serial.println("===============\n");
}

void loop() {
  // 检查各个连接状态
  // checkWiFiConnection();
  // checkMQTTConnection();
  
  // 处理MQTT消息
  // handleMQTTLoop();
  
  // 检查按钮状态
  // checkButton();
  
  // 发送心跳
  // checkHeartbeat();
  
  // 处理串口命令
  handleSerialCommand();
  
  // 更新LED状态（处理异步闪烁）
  updateLEDs();
  
  delay(10);
}
