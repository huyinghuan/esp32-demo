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
