#include "TaskManager.h"
#include "LedController.h"
#include "BrightnessController.h"
#include "RelaisController.h"
#include "PanelAngleController.h"
#include "PumpController.h"
#include "BatteryController.h"

TaskManager taskManager;

LedController ledController;
BrightnessController brightnessController;
RelaisController relaisController;
PanelAngleController panelAngleController(&brightnessController, &relaisController, &ledController);
BatteryController batteryController(&ledController);
PumpController pumpController(&relaisController, &ledController, &batteryController, &brightnessController);

void setup() {
  Serial.begin(115200);

  taskManager.registerTask(&ledController);
  taskManager.registerTask(&brightnessController);
  taskManager.registerTask(&relaisController);
  taskManager.registerTask(&panelAngleController);
  taskManager.registerTask(&batteryController);
  taskManager.registerTask(&pumpController);
  
  taskManager.init();
}

void loop() {
  taskManager.update();
  delay(10);
}
