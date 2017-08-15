#include "DisplayController.h"
#include "Debug.h"
#include <LogHelper.h>
#include <TimeLib.h>
#include <math.h>

#include "TaskIDs.h"

#include "ButtonController.h"
#include "BrightnessController.h"
#include "BatteryController.h"
#include "PanelAngleController.h"
#include "PumpController.h"
#include "TimeController.h"

DisplayController::DisplayController() : AbstractIntervalTask(1000) {
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  taskManager->getTask<ButtonController*>(TASK_BUTTON_CONTROLLER)->setButtonHandler(this);

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
      TimeController::TIME_STATE ts = taskManager->getTask<TimeController*>(TASK_TIME_CONTROLLER)->getState();
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

        bool hasDCF77Signal = taskManager->getTask<TimeController*>(TASK_TIME_CONTROLLER)->hasDCF77Signal();

        if (ts==TimeController::TIME_DCF77) {
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'C', false);
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'F', hasDCF77Signal);
        } else if (ts==TimeController::TIME_DS1307){
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'D', false);
          ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, '5', hasDCF77Signal);
        }
      }
      break;
    }
    case DC_BRIGHTNESS:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'b', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x05);
      printNumber(DEFAULT_DISPLAY_ADDR, taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->getSensorValue(), 0);
      break;
    case DC_DARK_LEVEL:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'D', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0xe);   // L
      printNumber(DEFAULT_DISPLAY_ADDR, taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->getDarkLevel(), 0);
      break;
    case DC_BATTERY:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'b', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'a', taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->isBatteryCritical());

      //printNumber(DEFAULT_DISPLAY_ADDR, (float)12.34, 0);
      printNumber(DEFAULT_DISPLAY_ADDR, taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->getVoltage(), 0);

      break;
    case DC_PANEL_ANGLE: {
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 6, 'a', false);
      bool printCountdown = true;
      
      if (taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->getMotorState()<0) {
        ledControl.setRow(DEFAULT_DISPLAY_ADDR,4,0x3e);     //u
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, ' ', false);
        //ledControl.setRow(DEFAULT_DISPLAY_ADDR,3,0x67);     //p
      } else if (taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->getMotorState()>0) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, 'd', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, ' ', false);
        //ledControl.setRow(DEFAULT_DISPLAY_ADDR, 3, 0x1d);   // o
      } else {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 4, ' ', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, ' ', false);
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 2, ' ', false);
        printCountdown = false;
      }

      if (printCountdown) {
        int v = taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->getAdjustTimeoutProgress();
        if (v>99) v = 99;
        printNumber(DEFAULT_DISPLAY_ADDR, 3, v);
      }

      ledControl.setDigit(DEFAULT_DISPLAY_ADDR, 0, taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->getState(), false);
      break;
    }
    case DC_PUMP:
      ledControl.setChar(DEFAULT_DISPLAY_ADDR, 7, 'p', false);
      ledControl.setRow(DEFAULT_DISPLAY_ADDR,6,0x1c);     // u

      if (taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->hasOverride()) {
        ledControl.setChar(DEFAULT_DISPLAY_ADDR, 3, 'o', false);
      } else {
        printNumber(DEFAULT_DISPLAY_ADDR, taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->getRemainingMinutes(), 3);
      }
      printBool(DEFAULT_DISPLAY_ADDR, taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->getState(), 0);
      break;
  }
}

void DisplayController::printBool(int addr, bool v, uint8_t offset) {
  if (v) {
    ledControl.setChar(addr, offset, '1', false);
  } else {
    ledControl.setChar(addr, offset, '0', false);
  }

  /*
  ledControl.setChar(addr, offset+2, '0', false);
  if (v) {
    ledControl.setChar(addr, offset+1, 'n', false);
    ledControl.setChar(addr, offset, ' ', false);
  } else {
    ledControl.setChar(addr, offset+1, 'f', false);
    ledControl.setChar(addr, offset, 'f', false);
  }*/
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
  
  LOG_PRINT(F("Displ "));
  LOG_PRINTLN(newDisplayOn);
  
  ledControl.shutdown(DEFAULT_DISPLAY_ADDR, !displayOn);
}

void DisplayController::onLeft(bool isDown) {
  onLeftRight(isDown, -1);
}

void DisplayController::onRight(bool isDown) {
  onLeftRight(isDown, 1);
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
      taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->overrideMotorState(isDown, directionUp);
      break;
    case DC_PUMP:
      taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->overrideState(isDown, directionUp);
      break;
  }
}

void DisplayController::onUp(bool isDown) {
  onUpDown(isDown, -1);
}

void DisplayController::onDown(bool isDown) {
  onUpDown(isDown, 1);
}

void DisplayController::onPressed(bool isDown) {
  onLeftRight(isDown, 1);
}

