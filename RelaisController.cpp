#include "RelaisController.h"
#include "Debug.h"
#include <LogHelper.h>

RelaisController::RelaisController() {
}

RelaisController::~RelaisController() {
}

void RelaisController::init() {
  //pinMode(PIN_RELAIS_MOTOR_ON, OUTPUT);
  pinMode(PIN_RELAIS_MOTOR_UP, OUTPUT);
  pinMode(PIN_RELAIS_MOTOR_DOWN, OUTPUT);
  pinMode(PIN_RELAIS_PUMP, OUTPUT);
  
  //digitalWrite(PIN_RELAIS_MOTOR_ON, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_UP, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_DOWN, HIGH);
  digitalWrite(PIN_RELAIS_PUMP, HIGH);
}

void RelaisController::update() {
}

void RelaisController::setState(uint8_t pin, bool state) {
  LOG_PRINT(F("Setting pin "));
  LOG_PRINT(pin);
  LOG_PRINT(F(" to "));
  LOG_PRINTLN(state);
  
  if (state) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

