#include "mqtt_message.h"
#include <managers/device_id_manager.h>

// 转换为JSON字符串
String MQTTMessage::toJSON() const {
  String json = "{";
  
  // 添加 event_type 字段（必填）
  json += "\"event_type\":\"" + event_type + "\"";
  
  // 添加device_id字段（必填）
  json += ",\"did\":" + String(getDeviceID());
  
  // 添加 sensor 字段（可选）
  if (sensor.length() > 0) {
    json += ",\"sensor\":\"" + sensor + "\"";
  }
  
  // 添加 value 字段（可选）
  if (value.length() > 0) {
    json += ",\"value\":\"" + value + "\"";
  }
  
  // 添加时间戳
  json += ",\"timestamp\":" + String(millis());
  
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