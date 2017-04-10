#include "BrightnessController.h"

BrightnessController::BrightnessController() : AbstractIntervalTask(UPDATE_BRIGHTNESS_INTERVAL_MS) {
}

BrightnessController::~BrightnessController() {
}

void BrightnessController::init() {
}

void BrightnessController::update2() {
  sensorValue = analogRead(PIN_BRIGHTNESS_SENSOR);
}

int BrightnessController::getSensorValue() {
  return sensorValue;
}

bool BrightnessController::isDark() {
  return sensorValue>BRIGHTNESS_DARK_LEVEL;
}

