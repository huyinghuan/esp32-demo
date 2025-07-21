
#include "mqtt_handle.h"
#include "../config.h"
#include "../managers/mqtt_manager.h"
#include "../output/status_manager.h"
#include "../devices/screen.h"

static unsigned long lastMsgAcceptTime = 0;

// MQTT消息回调函数
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    DEBUG_PRINT("收到MQTT消息 [");
    DEBUG_PRINT(topic);
    DEBUG_PRINT("]: ");
    lastMsgAcceptTime = millis();
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    DEBUG_PRINTLN(message);
    setShowingStatus(false);
    clearScreen();
    addText(message, 0, 15);
//   // 处理接收到的命令
//   if (String(topic) == sub_topic_command) {
//     handleCommand(message);
//   }

    handleCommand(message);
}

// mqtt 是否空闲
bool isMQTTIdle() {
    // 检查是否在最近10秒内接收到命令
    return (millis() - lastMsgAcceptTime) > 10000;
}

void handleCommand(String command) {
    // TODO 业务代码

//   if (command == "status") {
//   } else if (command == "reset_counter") {
//     buttonPressCount = 0;
//     publishMessage(pub_topic_status.c_str(), "counter_reset");
//     DEBUG_PRINTln("按钮计数器已重置");
//   } else if (command == "get_count") {
//     String countMsg = "button_count:" + String(buttonPressCount);
//     publishMessage(pub_topic_status.c_str(), countMsg.c_str());
//   } else if (command == "blink") {
//     blinkLED(5, 100);
//     publishMessage(pub_topic_status.c_str(), "blink_executed");
//   }
}