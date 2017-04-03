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
  
  digitalWrite(PIN_RELAIS_MOTOR_ON, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_UP, HIGH);
  digitalWrite(PIN_RELAIS_MOTOR_DOWN, HIGH);
}

void RelaisController::update() {
}

void RelaisController::setState(uint8_t pin, bool state) {
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

