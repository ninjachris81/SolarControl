#include "BatteryController.h"

BatteryController::BatteryController(LedController* ledController) : AbstractIntervalTask(UPDATE_BATTERY_INTERVAL_MS) {
  this->ledController = ledController;
}

BatteryController::~BatteryController() {
}

void BatteryController::init() {
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);
}

void BatteryController::update2() {
  float vout = (analogRead(PIN_BATTERY_VOLTAGE) * 5.0) / 1024.0;
  currentVoltage = vout / (R2/(R1+R2));
}

bool BatteryController::isUsingBattery() {
  return currentVoltage>11.0;
}

