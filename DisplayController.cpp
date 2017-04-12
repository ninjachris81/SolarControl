#include "DisplayController.h"
#include "Debug.h"

DisplayController::DisplayController(ButtonController* buttonController, BrightnessController* brightnessController, BatteryController* batteryController, PanelAngleController* panelAngleController, PumpController* pumpController, TimeController* timeController) : AbstractIntervalTask(1000) {
  this->buttonController = buttonController;
  this->brightnessController = brightnessController;
  this->batteryController = batteryController;
  this->panelAngleController = panelAngleController;
  this->pumpController = pumpController;

  this->buttonController->setJoystickHandler(this);
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, false);
  ledControl.setIntensity(DEFAULT_DISPLAY_ADDR, DISPLAY_BRIGHTNESS);
  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);
}

void DisplayController::update2() {
  Serial.println(displayTimeout, DEC);
  if (displayTimeout>0) displayTimeout--;
  setDisplayOn(displayTimeout>0);

  updateDisplay();
}



void DisplayController::updateDisplay() {
  if (!displayOn) return;

  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);

  //ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, displayContent, true);

  switch(displayContent) {
    case DC_TIME:
      if (timeController->isTimeSynced()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 5, 'n', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'o', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, '5', false);
        ledControl.setRow(DEFAULT_DISPLAY_ADDR, 2, 0x1c);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'n', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'c', false);
      } else {
        printNumber(DEFAULT_DISPLAY_ADDR, millis());
      }
      break;
    case DC_BRIGHTNESS:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'b', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x05);
      printNumber(DEFAULT_DISPLAY_ADDR, brightnessController->getSensorValue());
      break;
    case DC_BATTERY:
      if (batteryController->isUsingBattery()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'b', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'a', batteryController->isBatteryCritical());
      } else {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'a', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'c', false);
      }

      printNumber(DEFAULT_DISPLAY_ADDR, 12.34);
      //printNumber(DEFAULT_DISPLAY_ADDR, batteryController->getVoltage());
      break;
    case DC_PANEL_ANGLE:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'a', false);
      printNumber(DEFAULT_DISPLAY_ADDR, panelAngleController->getState());
      break;
    case DC_PUMP:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x1c);
      printBool(DEFAULT_DISPLAY_ADDR, pumpController->getState(), 0);
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
  if (hundreds>0) ledControl.setDigit(addr, 2, (byte)hundreds, false);
  if (tens>0 || hundreds>0) ledControl.setDigit(addr, 1, (byte)tens, false);
  ledControl.setDigit(addr, 0, (byte)ones, false);
}

void DisplayController::printNumber(int addr, float v) {
  float ones;
  float tens;
  float hundreds;

  float f1;
  float f2;
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
    ledControl.setChar(addr, 5, '-', false);
  }
  else {
    //print a blank in the sign column
    ledControl.setChar(addr, 5, ' ', false);
  }
  //Now print the number digit by digit
  ledControl.setDigit(addr, 2, (byte)hundreds, false);
  ledControl.setDigit(addr, 1, (byte)tens, false);
  ledControl.setDigit(addr, 0, (byte)ones, false);
}

void DisplayController::setDisplayOn(bool newDisplayOn) {
  if (displayOn==newDisplayOn) return;
  displayOn = newDisplayOn;
  
  Serial.print(F("Displ "));
  Serial.println(newDisplayOn);
  
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, !displayOn);
}

void DisplayController::onLeft(bool isDown) {
  if (isDown) {
    if (displayOn) {
      if (displayContent==DC_MIN) {
        displayContent = DC_MAX;
      } else {
        displayContent--;
      }
    }
    displayTimeout = DISPLAY_TIMEOUT_MS;
    Serial.print(F("Display Mode: "));
    Serial.println(displayContent, DEC);
  }
}

void DisplayController::onRight(bool isDown) {
  if (isDown) {
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
  }
}

void DisplayController::onUp(bool isDown) {
  
}

void DisplayController::onDown(bool isDown) {
  
}

void DisplayController::onPressed(bool isDown) {
  
}

