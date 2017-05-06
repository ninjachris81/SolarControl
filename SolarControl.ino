#include <LedControl.h>
#include <TimeLib.h>
#include <DCF77.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <TaskManager.h>
#include <SoftwareSerial.h>

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
#include "RemoteLogger.h"

TaskManager taskManager;

// order: see TaskIds.h
TimeController timeController;
ButtonController buttonController;
BrightnessController brightnessController;
LedController ledController;
RelaisController relaisController;
PanelAngleController panelAngleController;
BatteryController batteryController;
PumpController pumpController;
DisplayController displayController;
RemoteLogger remoteLogger;

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
  taskManager.registerTask(&remoteLogger);

  taskManager.init();
}

void loop() {
  taskManager.update();
  delay(100);
}
