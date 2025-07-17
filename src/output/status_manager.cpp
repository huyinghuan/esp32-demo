#include <Arduino.h>
#include "./status_manager.h"
#include <managers/wifi_manager.h>
#include <devices/screen.h>
#include <config.h>

static String wifiIP = ""; // WiFi IP地址
static int wifiSignal = 0; // WiFi信号强度

static unsigned long previousMillis = 0;
const unsigned long checkInterval = 5000; // 状态检查间隔时间（毫秒）
void printStatusToScreen(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis < checkInterval) {
        return;
    }
    previousMillis = currentMillis;

    if (getIP() != wifiIP || getWifiSignal() != wifiSignal) {
        wifiIP = getIP();
        wifiSignal = getWifiSignal();
        // 如果启用OLED屏幕，显示状态信息
        #if SCREEN_SSD1306_ENABLED
        clearScreen();
        addText("wifi: "+ getIP() + " " + String(getWifiSignal()), 0, 30);
        #else
            DEBUG_PRINTLN("WiFi IP: " + wifiIP);
            DEBUG_PRINTLN("信号强度: " + String(wifiSignal));
        #endif
    }
}