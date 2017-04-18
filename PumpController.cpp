#include "PumpController.h"

PumpController::PumpController(RelaisController* relaisController, LedController* ledController, BatteryController* batteryController, BrightnessController* brightnessController) : AbstractIntervalTask(UPDATE_PUMP_INTERVAL_MS) {
  this->relaisController = relaisController;
  this->ledController = ledController;
  this->batteryController = batteryController;
  this->brightnessController = brightnessController;
}

PumpController::~PumpController() {
}

void PumpController::init() {
  setState(false);
}

void PumpController::update2() {
  if (isOverride) return;
  
  if (brightnessController->isDark()) {
    Serial.println(F("It's dark"));

    if (lastToggle==0 || (pumpOn && (millis() - lastToggle > PUMP_STANDBY_INTERVAL_ON_MS))) {
      Serial.println(F("Pump intv off"));
      setState(false);
      lastToggle = millis();      
    } else if (lastToggle==0 || (!pumpOn && (millis() - lastToggle > PUMP_STANDBY_INTERVAL_OFF_MS))) {
      Serial.println(F("Pump intv on"));
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
    Serial.println(F("Batt critical - ignoring pump"));
    return;
  }
  
  if (this->pumpOn==pumpOn) return;
  
  Serial.print(F("Setting pump state to "));
  Serial.println(pumpOn);
  
  this->pumpOn = pumpOn;
  ledController->setState(INDEX_LED_PUMP_STATE, pumpOn ? LedController::LED_ON : LedController::LED_OFF);
  relaisController->setState(PIN_RELAIS_PUMP, pumpOn);
}

bool PumpController::getState() {
  return pumpOn;
}


