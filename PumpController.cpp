#include "PumpController.h"

PumpController::PumpController(RelaisController* relaisController, LedController* ledController, BatteryController* batteryController, BrightnessController* brightnessController) : AbstractIntervalTask(PUMP_STANDBY_INTERVAL_BATTERY_MS) {
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
  if (brightnessController->isDark()) {
    Serial.println(F("It's dark"));
    if (batteryController->isUsingBattery()) {
      setInterval(PUMP_STANDBY_INTERVAL_BATTERY_MS);
    } else {
      setInterval(PUMP_STANDBY_INTERVAL_AC_MS);
    }
  
    setState(!pumpOn);  
  } else {
    setState(true);
  }
}

void PumpController::setState(bool pumpOn) {
  if (this->pumpOn==pumpOn) return;
  
  Serial.print(F("Setting pump state to "));
  Serial.println(pumpOn);
  
  this->pumpOn = pumpOn;
  ledController->setState(INDEX_LED_PUMP_STATE, pumpOn);
  relaisController->setState(PIN_RELAIS_PUMP, pumpOn);
}

