#include "PumpController.h"
#include <LogHelper.h>

PumpController::PumpController(RelaisController* relaisController, LedController* ledController, BatteryController* batteryController, BrightnessController* brightnessController, TimeController* timeController) : AbstractIntervalTask(UPDATE_PUMP_INTERVAL_MS) {
  this->relaisController = relaisController;
  this->ledController = ledController;
  this->batteryController = batteryController;
  this->brightnessController = brightnessController;
  this->timeController = timeController;
}

PumpController::~PumpController() {
}

void PumpController::init() {
  setState(false);
}

void PumpController::update() {
  if (isOverride) return;
  bool doStandby = false;
  
  if ((timeController->getHourOfDay()<=6 && timeController->getHourOfDay()>=0) || timeController->getHourOfDay()>=20) {
    LOG_PRINT(F("It's night"));
    doStandby = true;
  } else if (brightnessController->isDark()) {
    LOG_PRINT(F("It's dark"));
    doStandby = true;
  }

  if (doStandby) {
    if (lastToggle==0 || (pumpOn && (millis() - lastToggle > PUMP_STANDBY_INTERVAL_ON_MS))) {
      LOG_PRINTLN(F("Pump intv off"));
      setState(false);
      lastToggle = millis();      
    } else if (lastToggle==0 || (!pumpOn && (millis() - lastToggle > PUMP_STANDBY_INTERVAL_OFF_MS))) {
      LOG_PRINTLN(F("Pump intv on"));
      setState(true);
      lastToggle = millis();
    }
  } else {
    setState(true);
    lastToggle = 0;
  }
}
bool PumpController::hasOverride() {
  return isOverride;
}

void PumpController::overrideState(bool isOverride, bool pumpOn) {
  this->isOverride = isOverride;
  setState(pumpOn);
}

void PumpController::setState(bool pumpOn) {
  if (!isOverride && batteryController->isBatteryCritical()) {
    LOG_PRINTLN(F("Batt critical - ignoring pump"));
    return;
  }
  
  if (this->pumpOn==pumpOn) return;
  
  LOG_PRINT(F("Setting pump state to "));
  LOG_PRINTLN(pumpOn);
  
  this->pumpOn = pumpOn;
  ledController->setState(INDEX_LED_PUMP_STATE, pumpOn ? LedController::LED_ON : LedController::LED_OFF);
  relaisController->setState(PIN_RELAIS_PUMP, pumpOn);
}

bool PumpController::getState() {
  return pumpOn;
}


