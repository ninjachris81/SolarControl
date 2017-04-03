#include "TaskManager.h"
#include "LedController.h"
#include "BrightnessController.h"
#include "RelaisController.h"
#include "PanelAngleController.h"
#include "PumpController.h"

TaskManager taskManager;
LedController ledController;
BrightnessController brightnessController;
RelaisController relaisController;
PanelAngleController panelAngleController(&brightnessController, &relaisController);
PumpController pumpController(&relaisController);

void setup() {
  Serial.begin(115200);

  taskManager.registerTask(&ledController);
  taskManager.registerTask(&brightnessController);
  taskManager.registerTask(&relaisController);
  taskManager.registerTask(&panelAngleController);
  taskManager.registerTask(&pumpController);
  
  taskManager.init();
}

void loop() {
  taskManager.update();
  //delay(10);
}
