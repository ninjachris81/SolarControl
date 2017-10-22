#include "BrightnessController.h"
#include <LogHelper.h>

BrightnessController::BrightnessController() : AbstractIntervalTask(UPDATE_BRIGHTNESS_INTERVAL_MS) {
  darkLevel = BRIGHTNESS_DARK_LEVEL;
  dayLevel = BRIGHTNESS_DAY_LEVEL;
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
  return getSensorValue()>darkLevel;
}

bool BrightnessController::isDay() {
  return getSensorValue()<dayLevel;
}

void BrightnessController::adjustLevels(bool darker) {
  if (darker) {
    darkLevel+=ADJUST_DELTA_UP;
    dayLevel+=ADJUST_DELTA_UP;
  } else {
    if (darkLevel<=BRIGHTNESS_DARK_LEVEL || dayLevel<=BRIGHTNESS_DAY_LEVEL) return;
    darkLevel-=ADJUST_DELTA_DOWN;
    dayLevel-=ADJUST_DELTA_DOWN;
  }
}

float BrightnessController::getDarkLevel() {
  return darkLevel;
}

float BrightnessController::getDayLevel() {
  return dayLevel;
}
