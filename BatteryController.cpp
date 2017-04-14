#include "BatteryController.h"

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
  return currentVoltage;
}

void BatteryController::update2() {
  float vout = (analogRead(PIN_BATTERY_VOLTAGE) * 5.0) / 1024.0;
  currentVoltage = vout / (R2/(R1+R2));

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
        Serial.println(F("USING BATT"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_OFF);
        break;
      case BATT_CRITICAL:
        Serial.println(F("BATT CRITICAL"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_BLINK);
        break;
    }
  }
}

bool BatteryController::isBatteryCritical() {
  return currentVoltage<CRITICAL_BATTERY_VOLTAGE;
}

