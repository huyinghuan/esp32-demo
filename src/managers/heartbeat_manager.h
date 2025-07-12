#ifndef HEARTBEAT_MANAGER_H
#define HEARTBEAT_MANAGER_H

#include <Arduino.h>

// 心跳管理函数
void initHeartbeat();
void checkHeartbeat();
void sendHeartbeat();

// 心跳状态变量
extern unsigned long lastHeartbeat;

#endif
