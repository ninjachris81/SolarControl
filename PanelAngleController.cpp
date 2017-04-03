#include "PanelAngleController.h"

PanelAngleController::PanelAngleController(BrightnessController* brightnessController, RelaisController* relaisController) : AbstractIntervalTask(UPDATE_PA_INTERVAL_MS) {
  this->brightnessController = brightnessController;
  this->relaisController = relaisController;
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
  
  int sve = brightnessController->getSensorValueEast();
  int svw = brightnessController->getSensorValueWest();

#ifdef IS_DEBUG
  Serial.print(F("EAST: "));
  Serial.println(sve, DEC);
  Serial.print(F("WEST: "));
  Serial.println(svw, DEC);
#endif

  if (sve<BRIGHTNESS_GLOBAL_THRESHOLD || svw<BRIGHTNESS_GLOBAL_THRESHOLD) {
    int delta = sve-svw;
    
    if (delta < BRIGHTNESS_DELTA_EAST) {
      newState = AS_EAST;
    } else if (delta > BRIGHTNESS_DELTA_WEST) {
      newState = AS_DEFAULT;
    } else {
      newState = AS_MIDDLE;
    }
  } else {
    newState = AS_DEFAULT;    // too dark, go default
  }

  lastStates[newState]++;

#ifdef IS_DEBUG
  Serial.print(F("Angle: "));
  Serial.println(newState, DEC);
#endif

  // check states commit
  if (lastStates[newState]>=LAST_STATES_MIN_COUNT) {
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

void PanelAngleController::setMotorState(bool doEnable, bool directionUp) {
#ifdef IS_DEBUG
  Serial.print(F("Setting motor state: "));
  Serial.print(doEnable);
  Serial.print(F(", direction: "));
  Serial.println(directionUp, DEC);
#endif
  
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

