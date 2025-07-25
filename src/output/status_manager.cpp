#include <Arduino.h>
#include "./status_manager.h"
#include <managers/wifi_manager.h>
#include <devices/screen.h>
#include <config.h>
#include <managers/mqtt_manager.h>

static String wifiIP = ""; // WiFi IP地址
static int wifiSignal = 0; // WiFi信号强度
static bool showingStatus = true; // 是否显示状态信息

static unsigned long previousMillis = 0;
const unsigned long checkInterval = 5000; // 状态检查间隔时间（毫秒）

// 设置是否显示状态信息
void setShowingStatus(bool show) {
    bool forceUpdate = false; // 不强制更新
    if (showingStatus != show && show) {
        #if SCREEN_SSD1306_ENABLED
        clearScreen();
        #endif
        forceUpdate = true; // 如果状态从隐藏变为显示，强制更新
    }
    showingStatus = show;
    if (show) {
        previousMillis = 0; // 重置计时器
        printStatusToScreen(forceUpdate);
    }
}
void printStatusToScreen(bool forceUpdate) {
    if (!showingStatus) {
        return; // 如果不需要显示状态信息，直接返回
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis < checkInterval && !forceUpdate) {
        return;
    }
    previousMillis = currentMillis;

    if (forceUpdate || (getIP() != wifiIP || getWifiSignal() != wifiSignal)){
        wifiIP = getIP();
        wifiSignal = getWifiSignal();
        // 如果启用OLED屏幕，显示状态信息
        #if SCREEN_SSD1306_ENABLED
        clearScreen();
        addText("wifi: "+ getIP() + " " + String(getWifiSignal()), 0, 15);
        #else
            DEBUG_PRINTLN("WiFi IP: " + wifiIP);
            DEBUG_PRINTLN("信号强度: " + String(wifiSignal));
        #endif
    }
    if(isMQTTConnected()){
        #if SCREEN_SSD1306_ENABLED
        addText("MQTT: Connected", 0, 30);
        #endif
    } else {
        #if SCREEN_SSD1306_ENABLED
        addText("MQTT: No Connect", 0, 30);
        #endif
    }
}