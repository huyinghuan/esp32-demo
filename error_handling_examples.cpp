// 改进的错误处理示例

// 方案1：使用重启
void initScreen() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        Serial.println(F("系统将在5秒后重启..."));
        delay(5000);
        ESP.restart(); // 重启ESP32
    }
    // ...其他初始化代码
}

// 方案2：返回错误状态
bool initScreen() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return false; // 返回失败状态
    }
    // ...其他初始化代码
    return true; // 返回成功状态
}

// 在setup()中处理错误
void setup() {
    if (!initScreen()) {
        Serial.println(F("屏幕初始化失败，进入安全模式"));
        // 进入安全模式，只执行基本功能
        for (;;) {
            // 可以闪烁LED表示错误状态
            digitalWrite(LED_PIN, HIGH);
            delay(500);
            digitalWrite(LED_PIN, LOW);
            delay(500);
        }
    }
    // 正常启动其他功能
}

// 方案3：设置错误标志
static bool screenAvailable = false;

void initScreen() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        screenAvailable = false;
        return;
    }
    screenAvailable = true;
    // ...其他初始化代码
}

// 在使用屏幕的函数中检查状态
void drawEmojiByType(int emojiType) {
    if (!screenAvailable) {
        Serial.println(F("屏幕不可用，跳过显示"));
        return;
    }
    // ...正常的显示代码
}
