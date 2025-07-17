#ifndef CONFIG_H
#define CONFIG_H

// ==================== 设备功能启用配置 ====================
// 添加调试开关
#ifndef MY_ESP_DEBUG
#define MY_ESP_DEBUG 1  // 0=关闭调试, 1=开启调试
#endif
#if MY_ESP_DEBUG
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINTLN(x)
#endif
#if MY_ESP_DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
#else
    #define DEBUG_PRINT(x)
#endif

#define BLUETOOTH_ENABLED false     // 是否启用蓝牙 设置为 false 可节省约 20-30mA 功耗// WiFi配置
#define WIFI_ENABLED true            // 是否启用WiFi
#define SCREEN_SSD1306_ENABLED true // 是否启用OLED屏幕

#define WIFI_POWER_SAVE_ENABLED false // 是否启用WiFi节能模式

#define MQTT_ENABLED false           // 是否启用MQTT

// 电源管理配置
#define POWER_MANAGEMENT_ENABLED true  // 是否启用电源管理
#define SMART_POWER_SAVE_MODE true  // 是否自动省电模式 电源管理

// MQTT主题模板（将根据设备ID动态生成）
// extern String pub_topic_button_press; 
extern String pub_topic_sensor; // 传感器主题
extern String pub_topic_status;
extern String sub_topic_command;

#define SUB_GLOBAL_TOPIC_COMMAND "esp32/command"  // 全局命令主题

// ===================== GPIO配置
extern const int buttonPin;
extern const int ledPin;

// 时间间隔配置
// extern const unsigned long wifiCheckInterval;
// extern const unsigned long mqttCheckInterval;
// extern const unsigned long heartbeatInterval;

// 设备ID相关函数
// void initDeviceTopics();
// void handleSerialCommand();

#endif // CONFIG_H
