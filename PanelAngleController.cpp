#include "PanelAngleController.h"
#include <LogHelper.h>

PanelAngleController::PanelAngleController(BrightnessController* brightnessController, RelaisController* relaisController, LedController* ledController, TimeController* timeController) : AbstractIntervalTask(UPDATE_PA_INTERVAL_MS) {
  this->brightnessController = brightnessController;
  this->relaisController = relaisController;
  this->ledController = ledController;
  this->timeController = timeController;
}

PanelAngleController::~PanelAngleController() {
}

void PanelAngleController::init() {
  resetLastStates();
  setMotorState(true, false);
}

void PanelAngleController::update() {
  if (isMotorOverride) return;
  
  if (isAdjustingAngle) {
    checkState();
  } else {
    checkNewState();
  }
}

void PanelAngleController::checkNewState() {
  PanelAngleController::ANGLE_STATE newState = AS_DEFAULT;
  
  int brightness = brightnessController->getSensorValue();

#ifdef IS_DEBUG
  LOG_PRINT(F("Brightness: "));
  LOG_PRINTLNF(brightness, DEC);
#endif

  if (brightness<BRIGHTNESS_GLOBAL_THRESHOLD) {

    if (timeController->getState()!=TimeController::TIME_INIT) {
      uint8_t h = timeController->getHourOfDay();
  
      if (h>=7 && h<=9) {    // 7 to 9
        newState = AS_EAST;
      } else if (h>=10 && h<=13) {   // 10 to 13
        newState = AS_MIDDLE;
      } else {
        newState = AS_DEFAULT;
      }
    } else {
      newState = AS_DEFAULT;    // no time
    }
  } else {
    newState = AS_DEFAULT;    // too dark, go default
  }

  lastStates[newState]++;
  lastStatesCount++;

#ifdef IS_DEBUG
  LOG_PRINT(F(", Angle: "));
  LOG_PRINTLNF(newState, DEC);
#endif

  if (lastStatesCount>=LAST_STATES_LIMIT) {
    resetLastStates();
  } else if (lastStates[newState]>=LAST_STATES_MIN_COUNT) {
    setState(newState);
    resetLastStates();
  }
}

void PanelAngleController::checkState() {
  adjustCountdown--;
  
#ifdef IS_DEBUG
  LOG_PRINT(F("Adjusting "));
  LOG_PRINTLNF(adjustCountdown, DEC);
#endif

  if (adjustCountdown<=0) {
    setMotorState(false);
    isAdjustingAngle = false;
  } else {
    // motor still adjusting
  }
}

void PanelAngleController::resetLastStates() {
  for (uint8_t i=0;i<PanelAngleController::AS_MAX;i++) lastStates[i] = 0;
  lastStatesCount = 0;
}

void PanelAngleController::setState(PanelAngleController::ANGLE_STATE state) {
  if (state==currentState) return;

  LOG_PRINT(F("New angle: "));
  LOG_PRINTF(state, DEC);

  isAdjustingAngle = true;
  int directionFactor = state - currentState;
  float c = 0.0;

  if (state==AS_EAST || currentState==AS_EAST) {
    c = ADJUST_COUNTDOWN_BIG;
  } else {
    c = ADJUST_COUNTDOWN_SMALL;
  }

  if (directionFactor>0) {    // up
    c = c * UP_FACTOR;
  }

  adjustCountdown = c;
  orgAdjustCountdown = adjustCountdown;

  if (directionFactor>0) {
    setMotorState(true, true);
  } else if (directionFactor<0) {
    setMotorState(true, false);
  }
  
  currentState = state;
}

int PanelAngleController::getMotorState() {
  return currentMotorState;
}

uint8_t PanelAngleController::getAdjustTimeoutProgress() {
  return 100.0 / ((float)orgAdjustCountdown / (float)adjustCountdown);
}

void PanelAngleController::setMotorState(bool doEnable) {
  setMotorState(doEnable, false);
}

PanelAngleController::ANGLE_STATE PanelAngleController::getState() {
  if (isMotorOverride) {
    return AS_OVERRIDE;
  } else {
    return currentState;
  }
}

void PanelAngleController::overrideMotorState(bool doEnable, bool directionUp) {
  isMotorOverride = doEnable;
  setMotorState(doEnable, directionUp);
}

void PanelAngleController::setMotorState(bool doEnable, bool directionUp) {
  LOG_PRINT(F("Setting motor state: "));
  LOG_PRINT(doEnable);
  LOG_PRINT(F(", direction: "));
  LOG_PRINTLNF(directionUp, DEC);

  if (!doEnable) {
    currentMotorState = 0;
  } else {
    if (directionUp) {
      currentMotorState = -1;
    } else {
      currentMotorState = 1;
    }
  }

  ledController->setState(INDEX_LED_MOTOR_STATE, doEnable ? LedController::LED_ON : LedController::LED_OFF);
  
  if (doEnable) {
    //relaisController->setState(PIN_RELAIS_MOTOR_ON, true);
    //delay(500);
    if (directionUp) {
      relaisController->setState(PIN_RELAIS_MOTOR_DOWN, false);
      delay(500);
      relaisController->setState(PIN_RELAIS_MOTOR_UP, true);
    } else {
      relaisController->setState(PIN_RELAIS_MOTOR_UP, false);
      delay(500);
      relaisController->setState(PIN_RELAIS_MOTOR_DOWN, true);
    }
  } else {
    //relaisController->setState(PIN_RELAIS_MOTOR_ON, false);
    //delay(500);
    relaisController->setState(PIN_RELAIS_MOTOR_DOWN, false);
    relaisController->setState(PIN_RELAIS_MOTOR_UP, false);
  }
}

