#include "PanelAngleController.h"

PanelAngleController::PanelAngleController(BrightnessController* brightnessController, RelaisController* relaisController, LedController* ledController, TimeController *timeController) : AbstractIntervalTask(UPDATE_PA_INTERVAL_MS) {
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

void PanelAngleController::update2() {
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

    // TODO: switch by time

    if (1==1) {
      newState = AS_EAST;
    } else if (1==2) {
      newState = AS_DEFAULT;
    } else {
      newState = AS_MIDDLE;
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

void PanelAngleController::setMotorState(bool doEnable) {
  setMotorState(doEnable, false);
}

PanelAngleController::ANGLE_STATE PanelAngleController::getState() {
  return currentState;
}


void PanelAngleController::setMotorState(bool doEnable, bool directionUp) {
#ifdef IS_DEBUG
  Serial.print(F("Setting motor state: "));
  Serial.print(doEnable);
  Serial.print(F(", direction: "));
  Serial.println(directionUp, DEC);
#endif

  ledController->setState(INDEX_LED_MOTOR_STATE, doEnable);
  
  if (doEnable) {
    relaisController->setState(PIN_RELAIS_MOTOR_ON, true);
    delay(500);
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
    relaisController->setState(PIN_RELAIS_MOTOR_ON, false);
    delay(500);
    relaisController->setState(PIN_RELAIS_MOTOR_DOWN, false);
    relaisController->setState(PIN_RELAIS_MOTOR_UP, false);
  }
}

