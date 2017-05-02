#include <LedControl.h>
#include <TimeLib.h>
#include <DCF77.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <TaskManager.h>

#define NO_LOG_SERIAL

#include <LogHelper.h>
#include "TimeController.h"
#include "LedController.h"
#include "BrightnessController.h"
#include "RelaisController.h"
#include "PanelAngleController.h"
#include "PumpController.h"
#include "BatteryController.h"
#include "ButtonController.h"
#include "DisplayController.h"

TaskManager taskManager;

TimeController timeController;
ButtonController buttonController;
BrightnessController brightnessController;
LedController ledController;
RelaisController relaisController;
PanelAngleController panelAngleController(&brightnessController, &relaisController, &ledController, &timeController);
BatteryController batteryController(&ledController, &relaisController);
PumpController pumpController(&relaisController, &ledController, &batteryController, &brightnessController, &timeController);
DisplayController displayController(&buttonController, &brightnessController, &batteryController, &panelAngleController, &pumpController, &timeController);

void setup() {
  LOG_INIT();

  LOG_PRINT(F("SETUP"));

  taskManager.registerTask(&timeController);
  taskManager.registerTask(&buttonController);
  taskManager.registerTask(&brightnessController);
  taskManager.registerTask(&ledController);
  taskManager.registerTask(&relaisController);
  taskManager.registerTask(&panelAngleController);
  taskManager.registerTask(&batteryController);
  taskManager.registerTask(&pumpController);
  taskManager.registerTask(&displayController);

  taskManager.init();
}

void loop() {
  taskManager.update();
  delay(100);
}
