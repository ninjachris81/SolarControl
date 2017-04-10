#include "DisplayController.h"
#include "Debug.h"

DisplayController::DisplayController(ButtonController* buttonController, BrightnessController* brightnessController, BatteryController* batteryController, PanelAngleController* panelAngleController, PumpController* pumpController) : AbstractIntervalTask(1000) {
  this->buttonController = buttonController;
  this->brightnessController = brightnessController;
  this->batteryController = batteryController;
  this->panelAngleController = panelAngleController;
  this->pumpController = pumpController;
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, false);
  ledControl.setIntensity(DEFAULT_DISPLAY_ADDR, DISPLAY_BRIGHTNESS);
  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);
}

void DisplayController::update2() {
  if (buttonController->consumeButtonClick()) {
    if (displayOn) {
      if (displayContent==DC_MAX) {
        displayContent = DC_MIN;
      } else {
        displayContent++;
      }
    }
    displayTimeout = DISPLAY_TIMEOUT_MS;
    Serial.print(F("Display Mode: "));
    Serial.println(displayContent, DEC);
  } else {
    Serial.println(displayTimeout, DEC);
    if (displayTimeout>0) displayTimeout--;
    setDisplayOn(displayTimeout>0);
  }

  updateDisplay();
}

void DisplayController::updateDisplay() {
  if (!displayOn) return;

  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);

  ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, displayContent, true);

  switch(displayContent) {
    case DC_TIME:
      printNumber(DEFAULT_DISPLAY_ADDR, millis());
      break;
    case DC_BRIGHTNESS:
      printNumber(DEFAULT_DISPLAY_ADDR, brightnessController->getSensorValue());
      break;
    case DC_BATTERY:
      if (batteryController->isUsingBattery()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'b', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'a', false);
      } else {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'a', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'c', false);
      }
      if (batteryController->isBatteryCritical()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'c', false);
      }
      break;
    case DC_PANEL_ANGLE:
      printNumber(DEFAULT_DISPLAY_ADDR, panelAngleController->getState());
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 5, 'p', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'a', false);
      break;
    case DC_PUMP:
      printBool(DEFAULT_DISPLAY_ADDR, pumpController->getState(), 0);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 5, 'p', false);
      break;
  }
}

void DisplayController::printBool(int addr, bool v, uint8_t offset) {
  ledControl.setChar(addr, offset+2, '0', false);
  if (v) {
    ledControl.setChar(addr, offset+1, 'n', false);
    ledControl.setChar(addr, offset, ' ', false);
  } else {
    ledControl.setChar(addr, offset+1, 'f', false);
    ledControl.setChar(addr, offset, 'f', false);
  }
}

void DisplayController::printNumber(int addr, int v) {
  int ones;
  int tens;
  int hundreds;
  boolean negative = false;

  if (v < -999 || v > 999)
    return;
  if (v < 0) {
    negative = true;
    v = v * -1;
  }
  ones = v % 10;
  v = v / 10;
  tens = v % 10;
  v = v / 10;
  hundreds = v;
  if (negative) {
    //print character '-' in the leftmost column
    ledControl.setChar(addr, 3, '-', false);
  }
  else {
    //print a blank in the sign column
    ledControl.setChar(addr, 3, ' ', false);
  }
  //Now print the number digit by digit
  ledControl.setDigit(addr, 2, (byte)hundreds, false);
  ledControl.setDigit(addr, 1, (byte)tens, false);
  ledControl.setDigit(addr, 0, (byte)ones, false);
}

void DisplayController::setDisplayOn(bool newDisplayOn) {
  if (displayOn==newDisplayOn) return;
  displayOn = newDisplayOn;
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, !displayOn);
}

