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
  bool doAdjustLevels = false;
  bool adjustLevelBrighter = false;

  TimeController* timeController = taskManager->getTask<TimeController*>(TASK_TIME_CONTROLLER);
  
  if ((timeController->getHourOfDay()<=6 && timeController->getHourOfDay()>=0) || timeController->getHourOfDay()>=20) {
    LOG_PRINTLN(F("It's night"));
    doStandby = true;
    currentStandbyOnIntervalMs = PUMP_STANDBY_INTERVAL_ON_MIN_MS;   // at night, use min on time
  } else if (taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->isBright()) {
    LOG_PRINTLN(F("It's bright"));
    doStandby = false;
  } else if (taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->isBatteryFull()) {
      LOG_PRINTLN(F("Battery full"));
      batteryFullTimeout = BATTERY_FULL_TIMEOUT;
      adjustLevelBrighter = true;
      doAdjustLevels = true;
  } else if (batteryFullTimeout>0) {
      batteryFullTimeout--;
      LOG_PRINTLN(F("Battery full decrease"));
  } else {
    // it's normal
    doStandby = true;
    adjustLevelBrighter = false;
    doAdjustLevels = true;
  }

  if (doAdjustLevels) {
    taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->adjustLevels(adjustLevelBrighter);
    if (adjustLevelBrighter) {
      currentStandbyOnIntervalMs+=PUMP_STANDBY_DELTA_CHANGE;    // make on interval bigger
    } else {
      currentStandbyOnIntervalMs-=PUMP_STANDBY_DELTA_CHANGE;    // make on interval smaller
    }
    currentStandbyOnIntervalMs = constrain(currentStandbyOnIntervalMs, PUMP_STANDBY_INTERVAL_ON_MIN_MS, PUMP_STANDBY_INTERVAL_ON_MAX_MS);
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

void PumpController::overrideState(bool isOverride, bool doPumpOn) {
  this->isOverride = isOverride;
  setState(doPumpOn);
}

void PumpController::setState(bool doPumpOn) {
  if (isOverride) {
    LOG_PRINTLN(F("OVERRIDE - ignoring pump"));
    return;
  }

  if (doPumpOn && taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->isBatteryCritical()) {
    LOG_PRINTLN(F("Batt critical - ignoring pump on"));
    doPumpOn = false;
  }
  
  if (this->pumpOn==doPumpOn) return;
  
  LOG_PRINT(F("Setting pump state to "));
  LOG_PRINTLN(doPumpOn);
  
  this->pumpOn = doPumpOn;
  taskManager->getTask<LedController*>(TASK_LED_CONTROLLER)->setState(INDEX_LED_PUMP_STATE, this->pumpOn ? LedController::LED_ON : LedController::LED_OFF);
  taskManager->getTask<RelaisController*>(TASK_RELAIS_CONTROLLER)->setState(PIN_RELAIS_PUMP, this->pumpOn);
}

bool PumpController::getState() {
  return this->pumpOn;
}

int PumpController::getRemainingMinutes() {
  return (millis() - lastToggle) / 60000;
}


