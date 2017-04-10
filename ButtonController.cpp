#include "ButtonController.h"

ButtonController::ButtonController() {
}

ButtonController::~ButtonController() {
}

void ButtonController::init() {
  pinMode(PIN_BUTTON, INPUT);
}

void ButtonController::update() {
  if (!buttonClicked) {
    if (digitalRead(PIN_BUTTON)) {
      buttonClicked = true;
    }
  }
}

bool ButtonController::consumeButtonClick() {
  if (buttonClicked) {
    buttonClicked = false;
    return true;
  } else {
    return false;
  }
}

