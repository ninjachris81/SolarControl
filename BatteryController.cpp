#include "BatteryController.h"

BatteryController::BatteryController(LedController* ledController) : AbstractIntervalTask(UPDATE_BATTERY_INTERVAL_MS) {
  this->ledController = ledController;
}

BatteryController::~BatteryController() {
}

void BatteryController::init() {
}

void BatteryController::update2() {
  
}

bool BatteryController::isUsingBattery() {
  return true;
}

