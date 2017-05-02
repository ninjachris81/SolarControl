#include "ButtonController.h"
#include <LogHelper.h>

ButtonController::ButtonController() {
}

ButtonController::~ButtonController() {
}

void ButtonController::init() {
}

void ButtonController::update() {
  keysHandler.update();
}

void ButtonController::setButtonHandler(FiveKeysHandler::FKFeedbackHandler *handler) {
  keysHandler.init(handler);
}
