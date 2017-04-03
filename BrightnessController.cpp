#include "BrightnessController.h"

BrightnessController::BrightnessController() : AbstractIntervalTask(UPDATE_BRIGHTNESS_INTERVAL_MS) {
}

BrightnessController::~BrightnessController() {
}

void BrightnessController::init() {
}

void BrightnessController::update2() {
  sensorValueEast = analogRead(PIN_SENSOR_EAST);
  sensorValueWest = analogRead(PIN_SENSOR_WEST);
}

int BrightnessController::getSensorValueEast() {
  return sensorValueEast;
}

int BrightnessController::getSensorValueWest() {
  return sensorValueWest;
}
