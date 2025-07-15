# ESP32 MQTT 物联网设备项目

一个基于ESP32的模块化MQTT物联网设备项目，支持按钮控制、LED指示、心跳监控和设备管理。

## ✨ 功能特性

- **WiFi连接管理** - 自动连接和重连机制
- **MQTT通信** - 支持发布/订阅消息
- **设备ID管理** - 持久化设备标识，支持多设备部署
- **按钮控制** - 防抖处理，发送按钮事件
- **LED指示** - 多LED管理，支持闪烁和状态指示
- **心跳监控** - 定期发送设备状态信息
- **统一消息格式** - 结构化JSON消息体
- **模块化架构** - 清晰的代码组织结构

## 📁 项目结构

```
src/
├── main.cpp                     # 主程序入口
├── config.h                     # 全局配置
├── secrets.h                    # 敏感配置（不会提交到Git）
├── managers/                    # 管理器模块
│   ├── wifi_manager.h/.cpp     # WiFi连接管理
│   ├── mqtt_manager.h/.cpp     # MQTT通信管理
│   ├── device_id_manager.h/.cpp # 设备ID管理
│   └── heartbeat_manager.h/.cpp # 心跳管理
├── devices/                    # 设备模块
│   ├── button.h/.cpp          # 按钮控制
│   └── led.h/.cpp             # LED管理
└── messages/                   # 消息模块
    ├── mqtt_message.h/.cpp    # MQTT消息结构
    └── messages.h             # 统一消息入口
```

## 🚀 快速开始

### 1. 克隆项目

```bash
git clone <your-repo-url>
cd esp32-mqtt-demo
```

### 2. 配置敏感信息

**重要：配置您的WiFi和MQTT信息**

```bash
# 复制示例配置文件
cp src/secrets.h.demo src/secrets.h
```

然后编辑 `src/secrets.h` 文件，填入您的实际配置：

```cpp
// WiFi配置
const char* WIFI_SSID = "您的WiFi名称";
const char* WIFI_PASSWORD = "您的WiFi密码";

// MQTT服务器配置
const char* MQTT_SERVER = "您的MQTT服务器IP";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "MQTT用户名";
const char* MQTT_PASSWORD = "MQTT密码";
```

### 3. 硬件连接

| 组件 | ESP32引脚 | 说明 |
|------|-----------|------|
| 按钮 | GPIO 14 | 内部上拉，按下接地 |
| LED | GPIO 2 | 内置LED或外接LED |

### 4. 编译和上传

使用PlatformIO编译和上传：

```bash
# 编译
pio run

# 上传到设备
pio run --target upload

# 查看串口输出
pio device monitor
```

## 📡 MQTT主题结构

设备会根据设备ID动态生成MQTT主题：

### 发布主题（设备发送）
- `esp32/{device_id}/sensor` - 按钮按下事件
- `esp32/{device_id}/status` - 设备状态和心跳

### 订阅主题（设备接收）
- `esp32/{device_id}/command` - 设备控制命令
- `esp32/command` - 全局控制命令

### 消息格式

所有消息都使用统一的JSON格式：

```json
{
  "event_type": "事件名称",
  "did": 1, // 设备id
  "sensor": "传感器",
  "value": "传感器值",
}
```

## 🔒 安全说明

- `src/secrets.h` 文件包含敏感信息，已添加到 `.gitignore` 中，不会提交到版本控制
- 请勿将WiFi密码和MQTT凭据硬编码在代码中
- 使用 `secrets.h.demo` 作为配置模板

## 🛠️ 开发说明

### 添加新设备

1. 在 `devices/` 文件夹中创建新的设备模块
2. 实现设备的初始化、控制和状态查询函数
3. 在 `main.cpp` 中调用设备的初始化函数

### 添加新管理器

1. 在 `managers/` 文件夹中创建新的管理器模块
2. 在 `managers/managers.h` 中添加包含语句
3. 实现管理器的业务逻辑

### 添加新消息类型

1. 在 `messages/` 文件夹中创建新的消息结构
2. 在 `messages/messages.h` 中添加包含语句
3. 实现消息的序列化和反序列化

## 📝 依赖库

- **PubSubClient** (v2.8.0) - MQTT客户端
- **WiFi** (v2.0.0) - WiFi连接
- **Preferences** (v2.0.0) - 持久化存储

## 🐛 故障排除

### WiFi连接问题
- 检查WiFi SSID和密码是否正确
- 确认ESP32在WiFi信号覆盖范围内
- 查看串口输出的连接状态

### MQTT连接问题
- 检查MQTT服务器地址和端口
- 验证MQTT用户名和密码
- 确认防火墙设置允许MQTT端口

### 编译问题
- 确保已安装PlatformIO
- 检查 `secrets.h` 文件是否存在
- 清理编译缓存：`pio run --target clean`

## 📄 许可证

MIT License

## 🤝 贡献

欢迎提交Issue和Pull Request！

---

**注意：首次使用前请务必配置 `src/secrets.h` 文件！**
