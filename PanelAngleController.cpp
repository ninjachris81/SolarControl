#include "PanelAngleController.h"

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
  Serial.print(F("Brightness: "));
  Serial.print(brightness, DEC);
#endif

  if (brightness<BRIGHTNESS_GLOBAL_THRESHOLD) {

    if (timeController->getState()!=TimeController::TIME_INIT) {
      uint8_t h = timeController->getHourOfDay();
  
      if (h>=8 && h<=10) {    // 8 to 10
        newState = AS_EAST;
      } else if (h>=11 && h<=14) {   // 11 to 14
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
  Serial.print(F(", Angle: "));
  Serial.println(newState, DEC);
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
  Serial.print(F("Adjusting "));
  Serial.println(adjustCountdown, DEC);
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

  Serial.print(F("New angle: "));
  Serial.println(state, DEC);

  isAdjustingAngle = true;
  int directionFactor = state - currentState;
  adjustCountdown = ADJUST_COUNTDOWN * abs(directionFactor);

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
#ifdef IS_DEBUG
  Serial.print(F("Setting motor state: "));
  Serial.print(doEnable);
  Serial.print(F(", direction: "));
  Serial.println(directionUp, DEC);
#endif
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

