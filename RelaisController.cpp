#include "RelaisController.h"
#include "Debug.h"

RelaisController::RelaisController() {
}

RelaisController::~RelaisController() {
}

void RelaisController::init() {
  pinMode(PIN_RELAIS_MOTOR_ON, OUTPUT);
  pinMode(PIN_RELAIS_MOTOR_UP, OUTPUT);
  pinMode(PIN_RELAIS_MOTOR_DOWN, OUTPUT);
  pinMode(PIN_RELAIS_PUMP, OUTPUT);
  
  digitalWrite(PIN_RELAIS_MOTOR_ON, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_UP, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_DOWN, HIGH);
  digitalWrite(PIN_RELAIS_PUMP, HIGH);

  // for testing - remove me
  digitalWrite(PIN_RELAIS_MOTOR_UP, LOW);
  digitalWrite(PIN_RELAIS_MOTOR_DOWN, LOW);
  digitalWrite(PIN_RELAIS_PUMP, LOW);
}

void RelaisController::update() {
}

void RelaisController::setState(uint8_t pin, bool state) {
  return;
  Serial.print(F("Setting pin "));
  Serial.print(pin);
  Serial.print(F(" to "));
  Serial.println(state);
  if (state) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

