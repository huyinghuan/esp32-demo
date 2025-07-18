#include <Arduino.h>
#include "wifi_handle.h"

#include <devices/led.h>

void handleWifiStatusChange(bool isConnected) {
    if(isConnected){
        blinkLEDAsync(ledPin, 1 , 100);
    }
}