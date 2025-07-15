#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "BluetoothSerial.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "../config.h"

// 蓝牙配置 - 从 config.h 读取配置
#define BLUETOOTH_ENABLED_BY_DEFAULT BLUETOOTH_ENABLED

// 蓝牙管理函数
void initBluetooth();
void initBluetoothIfEnabled();  // 根据配置决定是否初始化
void checkBluetoothConnection();
bool isBluetoothConnected();
bool isBluetoothEnabled();      // 检查蓝牙是否被启用
void enableBluetooth();         // 手动启用蓝牙
void disableBluetooth();        // 手动禁用蓝牙（节省功耗）
void stopBluetooth();
void restartBluetooth();
void sendBluetoothMessage(const String& message);
void handleBluetoothData();         // 处理接收到的蓝牙数据
void printBluetoothInfo();          // 打印详细状态信息

// 蓝牙状态变量
extern unsigned long lastBluetoothCheck;

#endif
