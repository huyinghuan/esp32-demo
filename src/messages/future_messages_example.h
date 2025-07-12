// 未来可以添加的其他消息类型示例

#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <Arduino.h>

// HTTP消息结构体示例
struct HTTPMessage {
  String method;        // GET, POST, PUT, DELETE
  String endpoint;      // API端点
  String payload;       // 请求体
  int status_code;      // 响应状态码
  
  // 构造函数
  HTTPMessage() : status_code(0) {}
  HTTPMessage(String m, String ep) : method(m), endpoint(ep), status_code(0) {}
  
  // 转换为HTTP格式
  String toHTTP() const;
};

// WebSocket消息结构体示例
struct WebSocketMessage {
  String event_type;
  String channel;
  String data;
  
  WebSocketMessage() {}
  WebSocketMessage(String type, String ch, String d) 
    : event_type(type), channel(ch), data(d) {}
  
  String toJSON() const;
};

// 串口消息结构体示例
struct SerialMessage {
  String command;
  String parameters;
  bool is_response;
  
  SerialMessage() : is_response(false) {}
  SerialMessage(String cmd, String params) 
    : command(cmd), parameters(params), is_response(false) {}
  
  String toString() const;
};

#endif
