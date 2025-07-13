#include "screen.h"

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
    
    // 初始化 U8g2（用于中文显示）
    u8g2.begin();
    u8g2.enableUTF8Print();  // 启用UTF8支持
    
    display.clearDisplay();
    
    // 使用U8g2显示中文和英文
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_chinese3);  // 使用支持中文的字体
    u8g2.setCursor(0, 15);
    u8g2.print("你好，ESP32！");
    u8g2.setCursor(0, 35);
    u8g2.setFont(u8g2_font_ncenB08_tr);  // 英文字体
    u8g2.print("Hello OLED!");
    u8g2.sendBuffer();
}

void displayChineseText(const char* text, int x, int y) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_chinese3);
    u8g2.setCursor(x, y);
    u8g2.print(text);
    u8g2.sendBuffer();
}

void displayMixedText(const char* line1, const char* line2, const char* line3) {
    u8g2.clearBuffer();
    
    // 第一行 - 中文字体
    u8g2.setFont(u8g2_font_wqy12_t_chinese3);
    u8g2.setCursor(0, 15);
    u8g2.print(line1);
    
    // 第二行 - 英文字体
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 35);
    u8g2.print(line2);
    
    // 第三行（可选）
    if (line3 != nullptr) {
        u8g2.setCursor(0, 50);
        u8g2.print(line3);
    }
    
    u8g2.sendBuffer();
}

// 绘制笑脸表情
void drawHappyFace() {
    u8g2.clearBuffer();
    
    // 脸部轮廓 (圆形) - 中心(64, 32), 半径28
    u8g2.drawCircle(64, 32, 28);
    
    // 左眼 (^)
    u8g2.setFont(u8g2_font_ncenB08_tr);  // 使用英文字体绘制 ^
    u8g2.setCursor(48, 28);
    u8g2.print("^");
    
    // 右眼 (^)
    u8g2.setCursor(72, 28);
    u8g2.print("^");
    
    // 鼻子 (.)
    u8g2.setCursor(62, 36);
    u8g2.print(".");
    
    // 笑脸嘴巴 (弧线) - 使用多个点连成弧形
    for (int x = 52; x <= 76; x += 2) {
        int y = 42 + (int)(4.0 * sin((x - 52) * 3.14159 / 24.0));
        u8g2.drawPixel(x, y);
        u8g2.drawPixel(x, y + 1); // 加粗线条
    }
    
    // 添加文字说明
    u8g2.setFont(u8g2_font_wqy12_t_chinese3);
    u8g2.setCursor(35, 58);
    // u8g2.print("开心！");
    
    u8g2.sendBuffer();
}

// 绘制平静脸表情
void drawNeutralFace() {
    u8g2.clearBuffer();
    
    // 脸部轮廓 (圆形) - 中心(64, 32), 半径28
    u8g2.drawCircle(64, 32, 28);
    
    // 左眼 (^)
    u8g2.setFont(u8g2_font_ncenB08_tr);  // 使用英文字体绘制 ^
    u8g2.setCursor(48, 28);
    u8g2.print("^");
    
    // 右眼 (^)
    u8g2.setCursor(72, 28);
    u8g2.print("^");
    
    // 鼻子 (.)
    u8g2.setCursor(62, 36);
    u8g2.print(".");
    
    // 平静嘴巴 (水平直线)
    u8g2.drawHLine(55, 42, 18);  // 从x=55到x=73的水平线
    u8g2.drawHLine(55, 43, 18);  // 加粗线条
    
    // 添加文字说明
    u8g2.setFont(u8g2_font_wqy12_t_chinese3);
    u8g2.setCursor(35, 58);
    // u8g2.print("平静~");
    
    u8g2.sendBuffer();
}

// 根据类型绘制表情
void drawEmojiByType(int emojiType) {
    switch (emojiType) {
        case 0:
            drawHappyFace();
            break;
        case 1:
            drawNeutralFace();
            break;
        default:
            drawHappyFace();  // 默认显示笑脸
            break;
    }
}