#include "mqtt_message.h"
#include <managers/device_id_manager.h>

// 转换为JSON字符串
String MQTTMessage::toJSON() const {
  String json = "{";
  
  // 添加event_type字段（必填）
  json += "\"e\":\"" + event_type + "\"";
  
  // 添加device_id字段（必填）
  json += ",\"did\":" + String(getDeviceID());
  
  // 添加sensor字段（可选）
  if (sensor.length() > 0) {
    json += ",\"s\":\"" + sensor + "\"";
  }
  
  // 添加value字段（可选）
  if (value.length() > 0) {
    json += ",\"v\":\"" + value + "\"";
  }
  
  // 添加时间戳
  json += ",\"t\":" + String(millis());
  
  json += "}";
  return json;
}

// 链式设置方法
MQTTMessage& MQTTMessage::setEventType(const String& type) {
  event_type = type;
  return *this;
}



MQTTMessage& MQTTMessage::setSensor(const String& sen) {
  sensor = sen;
  return *this;
}

MQTTMessage& MQTTMessage::setValue(const String& val) {
  value = val;
  return *this;
}

// 便捷的消息创建函数
MQTTMessage createButtonMessage(const String& event_type) {
  return MQTTMessage("button", event_type);
}

MQTTMessage createHeartbeatMessage() {
  return MQTTMessage("self", "heartbeat");
}