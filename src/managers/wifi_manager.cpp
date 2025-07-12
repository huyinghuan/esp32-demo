#include "wifi_manager.h"
#include "../config.h"
#include "../devices/led.h"

// WiFi状态变量
unsigned long lastWiFiCheck = 0;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  connectToWiFi();
}

void connectToWiFi() {
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    delay(500);
    Serial.print(".");
    retryCount++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi连接成功!");
    Serial.print("IP地址: ");
    Serial.println(WiFi.localIP());
    setLED(ledPin, true);  // WiFi连接成功，点亮LED
  } else {
    Serial.println();
    Serial.println("WiFi连接失败!");
    setLED(ledPin, false);
  }
}

void checkWiFiConnection() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastWiFiCheck >= wifiCheckInterval) {
    lastWiFiCheck = currentTime;
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi断开，尝试重连...");
      setLED(ledPin, false);
      connectToWiFi();
    }
  }
}

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
