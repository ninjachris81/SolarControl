#include "LedController.h"

LedController::LedController() : AbstractIntervalTask(BLINK_INTERVAL_MS) {
  for (uint8_t i = 0; i < LED_COUNT; i++) ledStates[i] = LED_OFF;
}

LedController::~LedController() {
}

void LedController::init() {
  pinMode(PIN_LED_MOTOR_STATE, OUTPUT);
  pinMode(PIN_LED_PUMP_STATE, OUTPUT);
  pinMode(PIN_LED_BATT_STATE, OUTPUT);

  setState(INDEX_LED_BATT_STATE, LED_ON);
  setState(INDEX_LED_MOTOR_STATE, LED_ON);
  setState(INDEX_LED_PUMP_STATE, LED_ON);

  delay(200);

  setState(INDEX_LED_BATT_STATE, LED_OFF);
  setState(INDEX_LED_MOTOR_STATE, LED_OFF);
  setState(INDEX_LED_PUMP_STATE, LED_OFF);
}

void LedController::update2() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    if (ledStates[i] == LED_BLINK) {
      int pin = getPin(i);
      digitalWrite(pin, !digitalRead(pin));
    }
  }
}

void LedController::setState(uint8_t index, LED_STATE state) {
  if (ledStates[index] == state) return;

  ledStates[index] = state;

  switch (state) {
    case LED_OFF:
      digitalWrite(getPin(index), LOW);
      break;
    case LED_ON:
      digitalWrite(getPin(index), HIGH);
      break;
  }
}

int LedController::getPin(uint8_t index) {
  switch (index) {
    case INDEX_LED_BATT_STATE:
      return PIN_LED_BATT_STATE;
    case INDEX_LED_MOTOR_STATE:
      return PIN_LED_MOTOR_STATE;
    case INDEX_LED_PUMP_STATE:
      return PIN_LED_PUMP_STATE;
  }

  return -1;
}
