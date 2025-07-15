#include "heartbeat_manager.h"
#include "../config.h"
#include "mqtt_manager.h"
#include "device_id_manager.h"
#include "../messages/messages.h"
#include <WiFi.h>

#if WIFI_ENABLED && MQTT_ENABLED
// 心跳状态变量
unsigned long lastHeartbeat = 0;

void initHeartbeat() {
  lastHeartbeat = millis();
}

void checkHeartbeat() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastHeartbeat >= heartbeatInterval) {
    lastHeartbeat = currentTime;
    sendHeartbeat();
  }
}

void sendHeartbeat() {
  if (!isMQTTConnected()) {
    return;
  }
  // 创建心跳消息
  MQTTMessage message = createHeartbeatMessage();
  
  // 添加额外的系统信息
  String systemInfo = "uptime:" + String(millis() / 1000) + 
                    ",wifi_rssi:" + String(WiFi.RSSI()) + 
                    ",free_heap:" + String(ESP.getFreeHeap());
  message.setValue(systemInfo);
  
  String jsonMessage = message.toJSON();
  publishMessage(pub_topic_status.c_str(), jsonMessage.c_str());
  
}
#endif
