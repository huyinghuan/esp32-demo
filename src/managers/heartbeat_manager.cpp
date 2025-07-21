#include "heartbeat_manager.h"
#include "mqtt_manager.h"
#include "device_id_manager.h"
#include "../messages/messages.h"
#include "../config.h"
#if WIFI_ENABLED && MQTT_ENABLED
// 心跳状态变量
unsigned long lastHeartbeat = 0;

void initHeartbeat() {
  lastHeartbeat = millis();
}
// 心跳间隔时间
// 注意mqtt服务器是否有超时断开配置，如果有的话那么需要相应调整心跳间隔，避免频繁断开
/**
 最优配置建议

参数	EMQX配置	ESP32	说明
Keep Alive	-	60秒	客户端承诺的最大静默时间
空闲超时	90秒	-	Keep Alive × 1.5 的缓冲
包活检查间隔	30秒	-	服务器检查频率
心跳间隔	-	30秒	Keep Alive 的 1/2
这样配置的优势：

稳定性好：有足够的缓冲时间应对网络抖动
资源效率：不会过于频繁地发送心跳
兼容性强：符合 MQTT 标准建议
 */
const unsigned long heartbeatInterval = 30000;

void checkHeartbeat() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastHeartbeat >= heartbeatInterval) {
    lastHeartbeat = currentTime;
    sendHeartbeat();
  }
}

void sendHeartbeat() {
  if (!isMQTTConnected()) {
    checkMQTTConnection();
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
