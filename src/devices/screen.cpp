#include "screen.h"
#include <managers/device_id_manager.h>
#include "../config.h"

#if SCREEN_SSD1306_ENABLED
// 定义全局display对象
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// 定义U8g2对象用于中文显示
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void initScreen() {
    // 初始化传统 Adafruit SSD1306
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        
        // 使用无限循环而不是return的原因：
        // 1. 嵌入式系统没有操作系统处理程序崩溃
        // 2. 如果return，程序会继续执行，可能使用未初始化的屏幕导致崩溃
        // 3. OLED屏幕是核心功能，初始化失败时应阻止程序继续运行
        // 4. 无限循环确保系统停在可预测的安全状态，便于调试
        for (;;); // 不继续执行
    }
    display.clearDisplay();
    // 初始化 U8g2（用于中文显示）
    u8g2.begin();
    u8g2.enableUTF8Print();  // 启用UTF8支持
    
    
    // U8g2 的字体名称中包含了尺寸信息，格式通常是：u8g2_font_[字体名]_[尺寸]_[字符集]
    // 字符集有 常见的如：chinese1 基础中文字符集, chinese2 扩展中文字符集, chinese3 更大字符集
    // 完整字符集: gb2312, gb2312a, gb2312b, 除非有必要显示很多中文，不建议使用更大的字符集，会增加flash占用
    // 纯英文字符集: u8g2_font_ncenB08_tf 比gb2312a小很多 falsh占用shao 20%，
    // 优化建议 如果要显示中文字符建议直行定制字库，只将需要的字符包含在内，减少不必要的字符集
    // 12号字体 Cursor Y = 15
    // 14号字体 Cursor Y = 18 width = 10.66  只能显示12个字符
    u8g2.clearBuffer(); // 清除U8g2缓冲区
    
    // 先尝试最基础的中文字体
    // u8g2.setFont(u8g2_font_wqy12_t_gb2312a); // 基础中文字符集
    
    // u8g2.setCursor(0, 15); // 设置光标位置
    // u8g2.print("ESP32"); // 先测试英文
    // 先尝试最基础的中文字体
    u8g2.setFont(u8g2_font_ncenB12_tf); // 基础中文字符集
    u8g2.setCursor(10, 15); // 第1行
    u8g2.print("starting..."); // 尝试其他常见中文字符
    u8g2.sendBuffer(); // 发送缓冲区内容到屏幕
}


// 软件关闭屏幕显示，保持I2C连接活跃，功耗最低
void turnOffScreen() {
    // 清除显示内容
    display.clearDisplay();
    display.display();
    
    // 发送显示关闭命令到SSD1306
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    
    // 清除U8g2缓冲区
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    
    Serial.println("屏幕显示已关闭");
}

// 重新开启屏幕显示
void turnOnScreen() {
    // 发送显示开启命令到SSD1306  
    display.ssd1306_command(SSD1306_DISPLAYON);
    
    // 刷新显示内容
    display.clearDisplay();
    display.display();
    
    Serial.println("屏幕显示已开启");
}

// 设置屏幕亮度 (0-255)
void setScreenBrightness(uint8_t brightness) {
    // SSD1306支持亮度调节，亮度越低功耗越小
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(brightness);
    
    Serial.print("屏幕亮度设置为: ");
    Serial.println(brightness);
}

// 屏幕进入深度睡眠模式 - 最大程度省电
void screenSleep() {
    // 先关闭显示
    turnOffScreen();
    
    // 进入睡眠模式，几乎停止所有功能
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    display.ssd1306_command(0x8E); // 关闭电荷泵
    
    Serial.println("屏幕进入深度睡眠");
}

// 从睡眠模式唤醒屏幕
void screenWakeup() {
    // 重新启用电荷泵
    display.ssd1306_command(0x8F); // 开启电荷泵
    
    // 重新初始化显示
    display.ssd1306_command(SSD1306_DISPLAYON);
    
    // 清除并刷新显示
    display.clearDisplay(); 
    display.display();
    
    Serial.println("屏幕从睡眠模式唤醒");
}

// 便捷的文本显示函数（清除屏幕后显示）
void displayText(String text, int x, int y) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tf);
    u8g2.setCursor(x, y);
    u8g2.print(text);
    u8g2.sendBuffer();
}

// 在现有内容基础上添加文本（不清除屏幕）
void addText(String text, int x, int y) {
    u8g2.setFont(u8g2_font_ncenB08_tf);
    u8g2.setCursor(x, y);
    u8g2.print(text);
    u8g2.sendBuffer();
}
void clearScreen() {
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}
#else

void initScreen(){}
// 省电相关函数
void turnOffScreen(){};         // 软件关闭显示
void turnOnScreen(){};          // 重新开启显示
void setScreenBrightness(uint8_t brightness){}; // 设置亮度 (0-255)
void screenSleep(){};           // 进入睡眠模式  
void screenWakeup(){};          // 从睡眠模式唤醒
void displayText(String text, int x = 0, int y = 15){}; // 显示文本的便捷函数
void addText(String text, int x = 0, int y = 15){}; // 在现有内容基础上添加文本
void clearScreen(){}; // 清除屏幕内容
#endif