#include "DisplayController.h"
#include "Debug.h"
#include <TimeLib.h>
#include <math.h>

DisplayController::DisplayController(ButtonController* buttonController, BrightnessController* brightnessController, BatteryController* batteryController, PanelAngleController* panelAngleController, PumpController* pumpController, TimeController* timeController) : AbstractIntervalTask(1000) {
  this->buttonController = buttonController;
  this->brightnessController = brightnessController;
  this->batteryController = batteryController;
  this->panelAngleController = panelAngleController;
  this->pumpController = pumpController;
  this->timeController = timeController;
  
  this->buttonController->setJoystickHandler(this);
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, false);
  ledControl.setIntensity(DEFAULT_DISPLAY_ADDR, DISPLAY_BRIGHTNESS);
  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);
}

void DisplayController::update() {
  if (displayTimeout>0) displayTimeout--;
  setDisplayOn(displayTimeout>0);
  updateDisplay();
}

void DisplayController::updateDisplay() {
  if (!displayOn) return;

  ledControl.clearDisplay(DEFAULT_DISPLAY_ADDR);

  switch(displayContent) {
    case DC_TIME: {
      TimeController::TIME_STATE ts = timeController->getState();
      if (ts==TimeController::TIME_INIT) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 5, 'n', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'o', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, '5', false);
        ledControl.setRow(DEFAULT_DISPLAY_ADDR, 2, 0x33);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 1, 'n', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 0, 'c', false);
      } else {
        if (hour()<10) printNumber(DEFAULT_DISPLAY_ADDR, 0, 3);
        printNumber(DEFAULT_DISPLAY_ADDR, hour(), 2, true);
        
        if (minute()<10) printNumber(DEFAULT_DISPLAY_ADDR, 0, 1);
        printNumber(DEFAULT_DISPLAY_ADDR, minute(), 0);

        if (ts==TimeController::TIME_DCF77) {
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'C', false);
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'F', false);
        } else if (ts==TimeController::TIME_DS1307){
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'D', false);
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, '5', false);
        }
      }
      break;
    }
    case DC_BRIGHTNESS:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'b', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x05);
      printNumber(DEFAULT_DISPLAY_ADDR, brightnessController->getSensorValue(), 0);
      break;
    case DC_BATTERY:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'b', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'a', batteryController->isBatteryCritical());

      //printNumber(DEFAULT_DISPLAY_ADDR, (float)12.34, 0);
      printNumber(DEFAULT_DISPLAY_ADDR, batteryController->getVoltage(), 0);

      break;
    case DC_PANEL_ANGLE:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'a', false);
      if (panelAngleController->getMotorState()<0) {
        ledControl.setRow(DEFAULT_DISPLAY_ADDR,4,0x3e);     //u
        ledControl.setRow(DEFAULT_DISPLAY_ADDR,3,0x67);     //p
      } else if (panelAngleController->getMotorState()>0) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'd', false);
        ledControl.setRow(DEFAULT_DISPLAY_ADDR, 3, 0x1d);
      } else {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, ' ', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, ' ', false);
      }

      ledControl.setDigit(DEFAULT_DISPLAY_ADDR, 0, panelAngleController->getState(), false);
      break;
    case DC_PUMP:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x1c);

      if (pumpController->hasOverride()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'o', false);
      } else {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, ' ', false);
      }
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

void DisplayController::printNumber(int addr, int v, uint8_t offset) {
  printNumber(addr, v, offset, false);
}

void DisplayController::printNumber(int addr, int v, uint8_t offset, bool withDot) {
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
    if (hundreds>0) {
      ledControl.setChar(addr, offset+3, '-', false);
    } else if (tens>0) {
      ledControl.setChar(addr, offset+2, '-', false);
    } else {
      ledControl.setChar(addr, offset+1, '-', false);
    }
  }
  
  //Now print the number digit by digit
  if (hundreds>0) ledControl.setDigit(addr, offset+2, (byte)hundreds, false);
  if (tens>0 || hundreds>0) ledControl.setDigit(addr, offset+1, (byte)tens, false);
  ledControl.setDigit(addr, offset, (byte)ones, withDot);
}

void DisplayController::printNumber(int addr, float v, uint8_t offset) {
  float ones;
  float tens;
  
  boolean negative = false;
  
  if (v < -999 || v > 999)
    return;
  if (v < 0) {
    negative = true;
    v = v * -1;
  }

  double d;
  int i;
  float fract = modf(v, &d);
  i = d;

  printNumber(addr, i, offset+2, true);

  i = fract*100;

  ones = i % 10;
  i = i / 10;
  tens = i % 10;
  
  ledControl.setDigit(addr, offset+1, (byte)tens, false);
  ledControl.setDigit(addr, offset, (byte)ones, false);
}

void DisplayController::setDisplayOn(bool newDisplayOn) {
  if (displayOn==newDisplayOn) return;
  displayOn = newDisplayOn;
  
  Serial.print(F("Displ "));
  Serial.println(newDisplayOn);
  
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, !displayOn);
}

void DisplayController::onLeft(bool isDown) {
  //onLeftRight(isDown, -1);
}

void DisplayController::onRight(bool isDown) {
  //onLeftRight(isDown, 1);
}

void DisplayController::onLeftRight(bool isDown, int dir){
  displayTimeout = DISPLAY_TIMEOUT_MS;

  if (isDown) {
    if (displayOn) {
      if (dir>0 && displayContent==DC_MAX) {
          displayContent = DC_MIN;
      } else if (dir<0 && displayContent==DC_MIN) {
          displayContent = DC_MAX;
      } else {
          displayContent+=dir;
      }
    }
  }
}

void DisplayController::onUpDown(bool isDown, int dir) {
  displayTimeout = DISPLAY_TIMEOUT_MS;
  bool directionUp;
  directionUp = dir<0;

  switch(displayContent) {
    case DC_TIME:
      break;
    case DC_BRIGHTNESS:
      break;
    case DC_BATTERY:
      break;
    case DC_PANEL_ANGLE:
      panelAngleController->overrideMotorState(isDown, directionUp);
      break;
    case DC_PUMP:
      pumpController->overrideState(isDown, directionUp);
      break;
  }
}

void DisplayController::onUp(bool isDown) {
  //onUpDown(isDown, -1);
}

void DisplayController::onDown(bool isDown) {
  //onUpDown(isDown, 1);
}

void DisplayController::onPressed(bool isDown) {
  onLeftRight(isDown, 1);
}

