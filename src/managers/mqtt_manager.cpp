#include "mqtt_manager.h"
#include "../config.h"
#include "wifi_manager.h"
#include "device_id_manager.h"
#include "../devices/led.h"

#if WIFI_ENABLED && MQTT_ENABLED

// MQTT客户端
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT状态变量
unsigned long lastMqttCheck = 0;

// 外部变量声明（在main.cpp中定义）
extern unsigned long buttonPressCount;

void initMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
  connectToMQTT();
}

void connectToMQTT() {
  if (!isWiFiConnected()) {
    return;
  }
  
  Serial.print("正在连接MQTT服务器: ");
  Serial.println(mqtt_server);
  
  // 尝试连接MQTT
  String clientID = generateClientID();
  if (client.connect(clientID.c_str(), mqtt_user, mqtt_pass)) {
    Serial.println("MQTT连接成功!");
    
    // 订阅命令主题
    client.subscribe(sub_topic_command.c_str());
    client.subscribe(SUB_GLOBAL_TOPIC_COMMAND);
    Serial.print("已订阅主题: ");
    Serial.println(sub_topic_command);
    Serial.println(SUB_GLOBAL_TOPIC_COMMAND);
    
    // LED闪烁表示MQTT连接成功
    blinkLED(ledPin, 3, 200);
    
  } else {
    Serial.print("MQTT连接失败, 错误码: ");
    Serial.println(client.state());
  }
}

void checkMQTTConnection() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastMqttCheck >= mqttCheckInterval) {
    lastMqttCheck = currentTime;
    
    if (!client.connected()) {
      connectToMQTT();
    }
  }
}

void handleMQTTLoop() {
  client.loop();
}

bool isMQTTConnected() {
  return client.connected();
}

void publishMessage(const char* topic, const char* message) {
  if (client.connected()) {
    if (client.publish(topic, message)) {
      Serial.print("发布消息到 ");
      Serial.print(topic);
      Serial.print(": ");
      Serial.println(message);
    } else {
      Serial.println("消息发布失败");
    }
  }
}

void publishStatus(const char* status) {
  publishMessage(pub_topic_status.c_str(), status);
}

// MQTT消息回调函数
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("收到MQTT消息 [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
//   // 处理接收到的命令
//   if (String(topic) == sub_topic_command) {
//     handleCommand(message);
//   }

  handleCommand(message);
}

void handleCommand(String command) {
    // TODO 业务代码

//   if (command == "status") {
//   } else if (command == "reset_counter") {
//     buttonPressCount = 0;
//     publishMessage(pub_topic_status.c_str(), "counter_reset");
//     Serial.println("按钮计数器已重置");
//   } else if (command == "get_count") {
//     String countMsg = "button_count:" + String(buttonPressCount);
//     publishMessage(pub_topic_status.c_str(), countMsg.c_str());
//   } else if (command == "blink") {
//     blinkLED(5, 100);
//     publishMessage(pub_topic_status.c_str(), "blink_executed");
//   }
}

#endif