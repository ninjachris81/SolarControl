#include "RemoteLogger.h"
#include <ESP8266_TS_Logger.h>
#include "BatteryController.h"
#include "BrightnessController.h"
#include "PanelAngleController.h"
#include "PumpController.h"

#include "TaskIDs.h"
#include <LogHelper.h>

RemoteLogger::RemoteLogger() : AbstractIntervalTask(LOGGER_INTERVAL_MS) {
}

void RemoteLogger::init() {
  ESP8266TsLogger::init(PIN_REMOTE_LOGGER_RX, PIN_REMOTE_LOGGER_TX, 115200);
}

void RemoteLogger::update() {
#ifdef IS_DEBUG
  String tmp = ESP8266TsLogger::readData();
  if (tmp.length()>0) LOG_PRINTLN(ESP8266TsLogger::readData());
#endif

  
  String postString = F("field1=");
  postString+=taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->getState();
  postString+=F("&field2=");
  postString+=taskManager->getTask<PanelAngleController*>(TASK_PANEL_ANGLE_CONTROLLER)->getState();
  postString+=F("&field3=");
  postString+=taskManager->getTask<BatteryController*>(TASK_BATTERY_CONTROLLER)->getVoltage();
  postString+=F("&field4=");
  postString+=taskManager->getTask<BrightnessController*>(TASK_BRIGHTNESS_CONTROLLER)->getSensorValue();
  postString+=F("&field5=");
  postString+=taskManager->getTask<PumpController*>(TASK_PUMP_CONTROLLER)->getRemainingMinutes();

  LOG_PRINT(F("Posting "));
  LOG_PRINTLN(postString);

  ESP8266TsLogger::postData(postString);
}

