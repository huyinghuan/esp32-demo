#include "device_id_manager.h"
#include "esp32_power_mgr.h"

// 通过串口命令设置设备ID的功能
void handleSerialCommand() {
    if(Serial.available() == 0){
        return; // 如果没有可用数据，直接返回
    }
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("SET_ID:")) {
        int newID = command.substring(7).toInt();
        if (newID > 0 && newID <= 9999) {
            setDeviceID(newID);
            Serial.println("设备ID已更新，重启后生效");
            Serial.println("新的客户端ID: " + generateClientID());
        } else {
            Serial.println("错误：设备ID必须在1-999之间");
        }
    } else if (command == "GET_ID") {
        Serial.println("当前设备ID: " + getDeviceIDString());
        Serial.println("客户端ID: " + generateClientID());
    } else if (command == "HELP") {
        Serial.println("可用命令:");
        Serial.println("SET_ID:123  - 设置设备ID为123");
        Serial.println("GET_ID      - 获取当前设备ID");
        Serial.println("HELP        - 显示帮助信息");
    }else if (command == "SLEEP"){
        enterESP32DeepSleep(0); // 立即进入深度睡眠
    } else {
        Serial.println("未知命令，请输入HELP获取帮助:"+command);
    }
}
