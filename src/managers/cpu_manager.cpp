#include "esp32_power_mgr.h"

void setESP32CPUFrequency(uint32_t freq) {
  setCpuFrequencyMhz(freq);
  Serial.print("CPU频率设置为: ");
  Serial.print(freq);
  Serial.println(" MHz");
}

void enableESP32CPUPowerSave() {
  setESP32CPUFrequency(80); // 降低到80MHz
  Serial.println("CPU节能模式: 已启用");
}

void disableESP32CPUPowerSave() {
  setESP32CPUFrequency(240); // 恢复到240MHz
  Serial.println("CPU节能模式: 已禁用");
}
