#include "BatteryController.h"
#include <LogHelper.h>

BatteryController::BatteryController(LedController* ledController, RelaisController* relaisController) : AbstractIntervalTask(UPDATE_BATTERY_INTERVAL_MS) {
  this->ledController = ledController;
  this->relaisController = relaisController;
}

BatteryController::~BatteryController() {
}

void BatteryController::init() {
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);
}

float BatteryController::getVoltage() {
  return currentVoltage.getValue() + VOLTAGE_CORRECTION;
}

void BatteryController::update() {
  float vout = (analogRead(PIN_BATTERY_VOLTAGE) * 4.3) / 1024.0;
  float newVoltage = vout / (R2/(R1+R2));

  currentVoltage.pushValue(newVoltage);

  BATT_STATE newState = BATT_INIT;

  if (isBatteryCritical()) {
    newState = BATT_CRITICAL;
  } else {
    newState = BATT_OK;
  }

  if (battState!=newState) {
    battState = newState;

    switch(battState) {
      case BATT_OK:
        LOG_PRINTLN(F("USING BATT"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_OFF);
        break;
      case BATT_CRITICAL:
        LOG_PRINTLN(F("BATT CRITICAL"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_BLINK);
        break;
    }
  }
}

bool BatteryController::isBatteryCritical() {
  return getVoltage()<CRITICAL_BATTERY_VOLTAGE;
}

