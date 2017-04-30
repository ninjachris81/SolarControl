#include "ButtonController.h"
#include <LogHelper.h>

ButtonController::ButtonController() {
}

ButtonController::~ButtonController() {
}

void ButtonController::init() {
}

void ButtonController::update() {
  joystick.update();
}

void ButtonController::setJoystickHandler(JoystickHandler::JoystickFeedbackHandler *handler) {
  joystick.setFeedbackHandler(handler);
}
