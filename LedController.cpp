#include "LedController.h"

LedController::LedController() : AbstractIntervalTask(BLINK_INTERVAL_MS) {
  for (uint8_t i=0;i<LED_COUNT;i++) blinkingLeds[i] = false;
}

LedController::~LedController() {
}

void LedController::init() {
  pinMode(PIN_LED_MOTOR_STATE, OUTPUT);
  pinMode(PIN_LED_PUMP_STATE, OUTPUT);
}

void LedController::update2() {
  for (uint8_t i=0;i<LED_COUNT;i++) {
    if (blinkingLeds[i]) {
      uint8_t ledState = digitalRead(i);
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
  
      digitalWrite(i, ledState);
    }
  }
}

void LedController::setState(uint8_t index, bool isOn) {
  digitalWrite(index, isOn);
}

void LedController::setBlinking(uint8_t index, bool isBlinking) {
  blinkingLeds[index] = isBlinking;
}
