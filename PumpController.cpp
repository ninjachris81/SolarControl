#include "PumpController.h"
#include <LogHelper.h>
#include "RelaisController.h"
#include "LedController.h"
#include "BatteryController.h"
#include "BrightnessController.h"
#include "TimeController.h"

#include "TaskIDs.h"

PumpController::PumpController() : AbstractIntervalTask(UPDATE_PUMP_INTERVAL_MS) {
}

PumpController::~PumpController() {
}

void PumpController::init() {
  setState(false);
}

void PumpController::update() {
  if (isOverride) return;
  bool doStandby = false;

  TimeController* timeController = taskManager->getTask<TimeController*>(TASK_TIME_CONTROLLER);
  
  if ((timeController->getHourOfDay()<=6 && timeController->getHourOfDay()>=0) || timeController->getHourOfDay()>=20) {
    LOG_PRINTLN(F("It's night"));
    doStandby = true;
    currentStandbyOnIntervalMs = PUMP_STANDBY_INTERVAL_ON_MIN_MS;
  } else if (taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->isDark() && !taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->isDay()) {
    LOG_PRINTLN(F("It's dark"));
    doStandby = true;
    if (currentStandbyOnIntervalMs>PUMP_STANDBY_INTERVAL_ON_MIN_MS) currentStandbyOnIntervalMs -=PUMP_STANDBY_DELTA_CHANGE;
  } else if (taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->isDay()) {
    if (taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->isBatteryFull()) {
      LOG_PRINTLN(F("Battery full"));
      batteryFullTimeout = BATTERY_FULL_TIMEOUT;
    } else {
      if (batteryFullTimeout>0) {
        batteryFullTimeout--;
        LOG_PRINTLN(F("Battery full timeout"));
      } else {
        LOG_PRINTLN(F("It's day"));
        doStandby = true;
        if (currentStandbyOnIntervalMs<PUMP_STANDBY_INTERVAL_ON_MAX_MS) currentStandbyOnIntervalMs+=PUMP_STANDBY_DELTA_CHANGE;
      }
    }
  } else {
    // it's bright
  }

  if (doStandby) {
    batteryFullTimeout = 0;
    
    if (lastToggle==0 || (pumpOn && (millis() - lastToggle > currentStandbyOnIntervalMs))) {
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
  if (isOverride) {
    LOG_PRINTLN(F("OVERRIDE - ignoring pump"));
    return;
  }

  if (pumpOn && taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->isBatteryCritical()) {
    LOG_PRINTLN(F("Batt critical - ignoring pump on"));
    return;
  }
  
  if (this->pumpOn==pumpOn) return;
  
  LOG_PRINT(F("Setting pump state to "));
  LOG_PRINTLN(pumpOn);
  
  this->pumpOn = pumpOn;
  taskManager->getTask<LedController*>(TASK_LED_CONTROLLER)->setState(INDEX_LED_PUMP_STATE, pumpOn ? LedController::LED_ON : LedController::LED_OFF);
  taskManager->getTask<RelaisController*>(TASK_RELAIS_CONTROLLER)->setState(PIN_RELAIS_PUMP, pumpOn);
}

bool PumpController::getState() {
  return pumpOn;
}

int PumpController::getRemainingMinutes() {
  return (millis() - lastToggle) / 60000;
}


