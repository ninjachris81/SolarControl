#include "BrightnessController.h"
#include <LogHelper.h>

BrightnessController::BrightnessController() : AbstractIntervalTask(UPDATE_BRIGHTNESS_INTERVAL_MS) {
  brightLevel = BRIGHTNESS_BRIGHT_LEVEL;
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

bool BrightnessController::isBright() {
  return getSensorValue() < BRIGHTNESS_BRIGHT_LEVEL;
}

bool BrightnessController::isDay() {
  return getSensorValue() < (BRIGHTNESS_BRIGHT_LEVEL + BRIGHTNESS_BRIGHT_LEVEL_DAY_ADD);
}

void BrightnessController::adjustLevels(bool brighter) {
  if (brighter) {
    brightLevel-=ADJUST_DELTA_UP;
  } else {
    brightLevel+=ADJUST_DELTA_DOWN;
  }

  brightLevel = constrain(brightLevel, BRIGHTNESS_BRIGHT_LEVEL_MIN, BRIGHTNESS_BRIGHT_LEVEL_MAX);
}

float BrightnessController::getBrightLevel() {
  return brightLevel;
}
