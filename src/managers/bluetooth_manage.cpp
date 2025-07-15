#include "bluetooth_manage.h"
#include "device_id_manager.h"

// 只有在启用蓝牙时才编译此文件
#if BLUETOOTH_ENABLED_BY_DEFAULT

// 蓝牙状态变量定义
unsigned long lastBluetoothCheck = 0;
bool bluetoothInitialized = false;
bool bluetoothConnected = false;
bool bluetoothEnabled = BLUETOOTH_ENABLED_BY_DEFAULT;  // 根据配置决定默认状态

// BluetoothSerial 对象
BluetoothSerial SerialBT;

// 蓝牙设备名称
String bluetoothDeviceName;

void initBluetoothIfEnabled() {
    if (bluetoothEnabled) {
        Serial.println("Bluetooth is enabled by configuration");
        initBluetooth();
    } else {
        Serial.println("Bluetooth is disabled by default to save power");
        Serial.println("Use enableBluetooth() to manually enable it later");
        
        // 确保蓝牙完全禁用以节省功耗
        disableBluetooth();
    }
}

void initBluetooth() {
    if (!bluetoothEnabled) {
        Serial.println("Cannot initialize: Bluetooth is disabled");
        return;
    }
    
    Serial.println("Initializing Bluetooth...");
    
    // 构建设备名称
    bluetoothDeviceName = "ESP32-Device-" + getDeviceIDString();
    
    // 初始化 BluetoothSerial
    if (!SerialBT.begin(bluetoothDeviceName.c_str())) {
        Serial.println("Failed to initialize Bluetooth");
        return;
    }
    
    bluetoothInitialized = true;
    Serial.println("Bluetooth initialized successfully");
    Serial.print("Device name: ");
    Serial.println(bluetoothDeviceName);
    Serial.println("The device started, now you can pair it with bluetooth!");
}

bool isBluetoothEnabled() {
    return bluetoothEnabled;
}

void enableBluetooth() {
    if (!bluetoothEnabled) {
        Serial.println("Enabling Bluetooth...");
        bluetoothEnabled = true;
        initBluetooth();
    } else {
        Serial.println("Bluetooth is already enabled");
    }
}

void disableBluetooth() {
    Serial.println("Disabling Bluetooth to save power...");
    
    // 停止蓝牙服务
    stopBluetooth();
    
    // 禁用蓝牙控制器以节省更多功耗
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    
    bluetoothEnabled = false;
    Serial.println("Bluetooth disabled successfully - power consumption reduced");
}

void checkBluetoothConnection() {
    // 如果蓝牙被禁用，跳过检查以节省CPU资源
    if (!bluetoothEnabled) {
        return;
    }
    
    unsigned long currentTime = millis();
    
    // 每5秒检查一次蓝牙连接状态
    if (currentTime - lastBluetoothCheck >= 5000) {
        lastBluetoothCheck = currentTime;
        
        if (!bluetoothInitialized) {
            bluetoothConnected = false;
            return;
        }
        
        // 检查是否有客户端连接
        bool currentConnectionStatus = SerialBT.hasClient();
        
        if (currentConnectionStatus != bluetoothConnected) {
            bluetoothConnected = currentConnectionStatus;
            
            if (bluetoothConnected) {
                Serial.println("Bluetooth client connected");
            } else {
                Serial.println("Bluetooth client disconnected");
            }
        }
        
        // 处理接收到的蓝牙数据
        handleBluetoothData();
    }
}

bool isBluetoothConnected() {
    return bluetoothConnected;
}

void stopBluetooth() {
    if (bluetoothInitialized) {
        SerialBT.end();
        bluetoothInitialized = false;
        bluetoothConnected = false;
        Serial.println("Bluetooth stopped");
    }
}

void restartBluetooth() {
    Serial.println("Restarting Bluetooth...");
    stopBluetooth();
    delay(1000);
    initBluetooth();
}


void sendBluetoothMessage(const String& message) {
    if (!bluetoothEnabled) {
        Serial.println("Cannot send message: Bluetooth is disabled");
        return;
    }
    
    if (bluetoothInitialized && bluetoothConnected) {
        SerialBT.println(message);
        Serial.print("Sent via Bluetooth: ");
        Serial.println(message);
    } else {
        Serial.println("Cannot send message: Bluetooth not connected");
    }
}

void printBluetoothInfo() {
    Serial.println("=== Bluetooth Status ===");
    Serial.print("Enabled: ");
    Serial.println(bluetoothEnabled ? "Yes" : "No (Power Save)");
    Serial.print("Initialized: ");
    Serial.println(bluetoothInitialized ? "Yes" : "No");
    Serial.print("Connected: ");
    Serial.println(bluetoothConnected ? "Yes" : "No");
    
    if (bluetoothEnabled && bluetoothInitialized) {
        Serial.print("Device Name: ");
        Serial.println(bluetoothDeviceName);
    }
    
    Serial.println("======================");
}


void handleBluetoothData() {
    // 检查是否有蓝牙数据可读
    if (SerialBT.available()) {
        String receivedData = SerialBT.readString();
        receivedData.trim(); // 移除首尾空白字符
        
        Serial.print("Received via Bluetooth: ");
        Serial.println(receivedData);
        
        // 这里可以添加更复杂的数据处理逻辑
        // 例如：解析命令、处理JSON数据等
        
        // 简单的回显功能
        SerialBT.print("Echo: ");
        SerialBT.println(receivedData);
        
        // 可以在这里添加其他处理逻辑，比如：
        // - 解析特定命令
        // - 转发到MQTT
        // - 控制设备状态等
    }
}

#endif