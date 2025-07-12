#ifndef DEVICE_ID_MANAGER_H
#define DEVICE_ID_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>

// 设备ID管理函数
void initDeviceID();
void setDeviceID(int deviceID);
int getDeviceID();
String getDeviceIDString();
String generateClientID();
bool hasDeviceID();

// 串口命令处理
void handleSerialCommand();

// 设备ID相关常量
#define DEFAULT_DEVICE_ID 1
#define DEVICE_ID_KEY "device_id"
#define DEVICE_ID_NAMESPACE "esp32_config"

#endif
