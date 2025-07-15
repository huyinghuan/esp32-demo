#include "device_id_manager.h"

// Preferences对象用于存储设备ID
Preferences preferences;

void initDeviceID() {
  // 初始化Preferences
  preferences.begin(DEVICE_ID_NAMESPACE, false);
  
  // 检查是否已经有设备ID
  if (!preferences.isKey(DEVICE_ID_KEY)) {
    // 如果没有设备ID，设置默认值
    Serial.println("首次启动，设置默认设备ID: " + String(DEFAULT_DEVICE_ID));
    setDeviceID(DEFAULT_DEVICE_ID);
  }
  
  int currentID = getDeviceID();
  Serial.println("设备ID: " + String(currentID));
  Serial.println("客户端ID: " + generateClientID());
}

void setDeviceID(int deviceID) {
  preferences.putInt(DEVICE_ID_KEY, deviceID);
  Serial.println("设备ID已设置为: " + String(deviceID));
}

int getDeviceID() {
  return preferences.getInt(DEVICE_ID_KEY, DEFAULT_DEVICE_ID);
}

String getDeviceIDString() {
  return String(getDeviceID());
}

String generateClientID() {
  return "ESP32_Device_" + getDeviceIDString();
}

bool hasDeviceID() {
  return preferences.isKey(DEVICE_ID_KEY);
}

// 通过串口命令设置设备ID的功能
void handleSerialCommand() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("SET_ID:")) {
      int newID = command.substring(7).toInt();
      if (newID > 0 && newID <= 9999) {
        setDeviceID(newID);
        Serial.println("设备ID已更新，重启后生效");
        Serial.println("新的客户端ID: " + generateClientID());
      } else {
        Serial.println("错误：设备ID必须在1-999之间");
      }
    } else if (command == "GET_ID") {
      Serial.println("当前设备ID: " + getDeviceIDString());
      Serial.println("客户端ID: " + generateClientID());
    } else if (command == "HELP") {
      Serial.println("可用命令:");
      Serial.println("SET_ID:123  - 设置设备ID为123");
      Serial.println("GET_ID      - 获取当前设备ID");
      Serial.println("HELP        - 显示帮助信息");
    }
  }
}
