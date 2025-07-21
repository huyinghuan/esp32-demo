#include <Arduino.h>
#include "wifi_handle.h"

#include <devices/led.h>

void handleWifiStatusChange(WiFiStatus status) {
    if(status == WIFI_CONNECTED){
        blinkLEDAsync(ledPin, 1 , 100);
    }
}