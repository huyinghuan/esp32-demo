#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <Arduino.h>

// MQTT消息结构体
struct MQTTMessage {
  int device_id;        // 设备ID
  String sensor;        // 传感器类型：temperature, humidity, button
  String event_type;    // 事件类型：button, heartbeat, sensor, status等
  String value;         // 值：on/off, 温度数值, 按钮状态等（可选）
  
  // 构造函数
  MQTTMessage() : device_id(0) {}
  
  MQTTMessage(String sensor, String event_type, String val = "") 
    : sensor(sensor), event_type(event_type), value(val){}
  
  
  // 转换为JSON字符串
  String toJSON() const;
  
  // 设置字段的便捷方法
  MQTTMessage& setEventType(const String& type);
  MQTTMessage& setSensor(const String& sen);
  MQTTMessage& setValue(const String& val);
};

// 便捷的消息创建函数
MQTTMessage createButtonMessage(const String& event_type);
MQTTMessage createHeartbeatMessage();

#endif
