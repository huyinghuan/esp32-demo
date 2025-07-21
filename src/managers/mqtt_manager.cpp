#include "mqtt_manager.h"
#include "wifi_manager.h"
#include "device_id_manager.h"
#include "../secrets.h"
#include "../config.h"

// 选择性编译降低flash占用和功耗
#if WIFI_ENABLED && MQTT_ENABLED

const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_pass = MQTT_PASSWORD;

const unsigned long mqttCheckInterval = 1000;

// MQTT客户端
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT状态变量
unsigned long lastMqttCheck = 0;


void initMQTT(MqttCallback callback) {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  connectToMQTT();
}

void connectToMQTT() {
  if (!isWiFiConnected()) {
    DEBUG_PRINT("wifi未连接 ");
    return;
  }
  
  DEBUG_PRINT("正在连接MQTT服务器: ");
  DEBUG_PRINTLN(mqtt_server);
  
  // 尝试连接MQTT
  String clientID = generateClientID();
  if (client.connect(clientID.c_str(), mqtt_user, mqtt_pass)) {
    DEBUG_PRINTLN("MQTT连接成功!");
    
    // 订阅命令主题
    client.subscribe(sub_topic_command.c_str());
    client.subscribe(SUB_GLOBAL_TOPIC_COMMAND);
    DEBUG_PRINT("已订阅主题: ");
    DEBUG_PRINTLN(sub_topic_command);
    DEBUG_PRINTLN(SUB_GLOBAL_TOPIC_COMMAND);

    // LED闪烁表示MQTT连接成功
    // blinkLED(ledPin, 3, 200);
    
  } else {
    DEBUG_PRINT("MQTT连接失败, 错误码: ");
    DEBUG_PRINTLN(client.state());
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

bool publishMessage(const char* topic, const char* message) {
  if (!client.connected()) {
    DEBUG_PRINTLN("MQTT未连接，无法发布消息");
    return false;
  }
  if (client.publish(topic, message)) {
    DEBUG_PRINT("发布消息到 ");
    DEBUG_PRINT(topic);
    DEBUG_PRINT(": ");
    DEBUG_PRINTLN(message);
    return  true;
  } 
  DEBUG_PRINTLN("消息发布失败");
  return false;
}

void publishStatus(const char* status) {
  publishMessage(pub_topic_status.c_str(), status);
}

#else
void initMQTT(MqttCallback callback) {
  DEBUG_PRINTLN("MQTT未启用，跳过初始化");
}
bool isMQTTConnected(){
  return false;
}
bool publishMessage(const char* topic, const char* message){return false;}

#endif