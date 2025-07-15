# ESP32 WiFi功耗优化指南

## 📋 概述

本项目已经集成了完整的WiFi功耗优化方案，包括智能WiFi管理、多级电源管理和深度睡眠功能。通过合理配置这些功能，可以显著降低ESP32的功耗。

## 🔧 功耗优化配置

### 1. 基本开关配置 (config.h)

```cpp
// WiFi功耗相关
#define WIFI_ENABLED true               // 启用/禁用WiFi
#define WIFI_POWER_SAVE_ENABLED true    // 启用WiFi节能模式
#define WIFI_SMART_MANAGEMENT true      // 启用智能WiFi管理

// 电源管理相关
#define POWER_MANAGEMENT_ENABLED true   // 启用电源管理
#define DEEP_SLEEP_ENABLED true         // 启用深度睡眠
#define CPU_POWER_SAVE_ENABLED true     // 启用CPU节能模式

// 其他模块
#define BLUETOOTH_ENABLED false         // 禁用蓝牙可节省20-30mA
```

### 2. WiFi功耗模式

项目支持4种WiFi功耗模式：

- **WIFI_POWER_FULL**: 全功率模式 (~80-120mA)
- **WIFI_POWER_MODEM**: 调制解调器睡眠模式 (~20-30mA)
- **WIFI_POWER_LIGHT**: 轻度睡眠模式 (~15-25mA)
- **WIFI_POWER_OFF**: 完全关闭WiFi (~5-10mA)

### 3. 电源管理模式

- **POWER_MODE_ACTIVE**: 活跃模式
- **POWER_MODE_LIGHT_SLEEP**: 轻度睡眠 (~0.8mA)
- **POWER_MODE_DEEP_SLEEP**: 深度睡眠 (~10μA)
- **POWER_MODE_HIBERNATION**: 休眠模式 (~2.5μA)

## 📊 功耗对比

| 模式 | 典型功耗 | 使用场景 |
|------|----------|----------|
| 全功率WiFi + 活跃CPU | 80-120mA | 持续网络通信 |
| 节能WiFi + 活跃CPU | 20-30mA | 间歇性网络通信 |
| WiFi关闭 + 活跃CPU | 5-10mA | 本地处理 |
| 轻度睡眠 | 0.8mA | 短期待机 |
| 深度睡眠 | 10μA | 长期待机 |
| 休眠模式 | 2.5μA | 极长期待机 |

## 🚀 使用方法

### 1. 启用基本功耗优化

在 `config.h` 中设置：
```cpp
#define WIFI_POWER_SAVE_ENABLED true
#define POWER_MANAGEMENT_ENABLED true
#define CPU_POWER_SAVE_ENABLED true
```

### 2. 手动控制WiFi功耗模式

```cpp
// 在你的代码中调用
setWiFiPowerMode(WIFI_POWER_MODEM);  // 设置为调制解调器睡眠模式
```

### 3. 手动进入睡眠模式

```cpp
// 轻度睡眠5秒
enterLightSleep(5 * 1000000);

// 深度睡眠30秒
enterDeepSleep(30 * 1000000);
```

### 4. 智能功耗管理

启用后，系统会自动：
- 根据网络活动调整WiFi功耗模式
- 在空闲时自动进入睡眠模式
- 在需要时自动唤醒

## ⚙️ 高级配置

### 1. 自定义睡眠阈值

在 `power_manager.cpp` 中修改：
```cpp
const unsigned long DEEP_SLEEP_THRESHOLD = 600000;    // 10分钟进入深度睡眠
const unsigned long LIGHT_SLEEP_THRESHOLD = 60000;    // 1分钟进入轻度睡眠
```

### 2. 自定义CPU频率

```cpp
setCPUFrequency(80);   // 设置为80MHz (默认240MHz)
```

### 3. 配置唤醒源

```cpp
// 定时器唤醒
setupWakeupTimer(30 * 1000000);  // 30秒后唤醒

// GPIO唤醒
setupWakeupPin(GPIO_NUM_14, 0);  // 引脚14低电平唤醒
```

## 📈 优化建议

### 2. 硬件优化建议

- 使用外部拉电阻而不是内部拉电阻
- 关闭不使用的LED和显示屏
- 选择低功耗的外围器件
- 优化PCB布局减少漏电流

### 3. 软件优化建议

- 减少不必要的串口输出
- 优化循环中的delay时间
- 合理使用中断而不是轮询
- 批量处理网络请求

## 🔍 功耗监控

### 1. 串口监控

系统会自动输出功耗相关信息：
```
WiFi: 调制解调器睡眠模式
CPU频率设置为: 80 MHz
检测到短期空闲，进入轻度睡眠
```

### 2. 功耗测量

建议使用万用表或专业功耗分析仪在以下点测量：
- VCC输入端
- 3.3V稳压器输出端
- ESP32芯片VDD引脚

## ⚠️ 注意事项

1. **深度睡眠会重启程序** - 从深度睡眠唤醒相当于重启，需要重新初始化
2. **WiFi重连时间** - 从睡眠唤醒后WiFi重连需要时间，影响实时性
3. **定时器精度** - 长时间睡眠可能存在定时器漂移
4. **外设状态** - 进入睡眠前需要正确保存外设状态

## 🎯 预期效果

正确配置后，功耗优化效果：

- **基础优化**: 功耗降低50-70%
- **智能管理**: 在保持功能的前提下，平均功耗降低80%
- **深度睡眠**: 待机功耗降低99%以上

根据你的具体应用场景，选择合适的优化策略，可以显著延长电池续航时间。
