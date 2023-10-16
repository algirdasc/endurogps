#include <Arduino.h>

#include "EnduroGPS.h"
#include "Battery.h"

float Battery::voltage()
{
  return analogRead(GPIO_BATTERY) / 4096.0 * 7.445;
}

uint8_t Battery::percentage()
{
  // use as LiPoChargePercentage(ReadBatteryVoltage())
  // LOLIN D32 PRO uses a TP4054 so max V should be 4.2V https://datasheetspdf.com/pdf/1090540/NanJingTopPower/TP4054/1
  // 0% <= 3.3
  // 100% > 4.2
  uint8_t batteryPercent;
  float normalizedCharge = (Battery::voltage() - 3.3) / (4.2 - 3.3) * 100;

  if (normalizedCharge > 100) {
    batteryPercent = 100;
  } else if (normalizedCharge < 0) {
    batteryPercent = 0;
  } else {
    batteryPercent = (uint8_t) normalizedCharge;
  }
  
  return batteryPercent;
}
