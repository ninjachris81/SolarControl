#include "BrightnessController.h"
#include <LogHelper.h>

BrightnessController::BrightnessController() : AbstractIntervalTask(UPDATE_BRIGHTNESS_INTERVAL_MS) {
}

BrightnessController::~BrightnessController() {
}

void BrightnessController::init() {
}

void BrightnessController::update() {
  sensorValue.pushValue(analogRead(PIN_BRIGHTNESS_SENSOR));
}

int BrightnessController::getSensorValue() {
  return sensorValue.getValue();
}

bool BrightnessController::isDark() {
  return getSensorValue()>BRIGHTNESS_DARK_LEVEL;
}

bool BrightnessController::isDay() {
  return getSensorValue()>BRIGHTNESS_DAY_LEVEL;
}
