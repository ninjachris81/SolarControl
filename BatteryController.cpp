#include "BatteryController.h"

BatteryController::BatteryController(LedController* ledController, RelaisController* relaisController) : AbstractIntervalTask(UPDATE_BATTERY_INTERVAL_MS) {
  this->ledController = ledController;
  this->relaisController = relaisController;
  for (uint8_t i=0;i<CURRENT_VOLTAGES_BUFFER_SIZE;i++) currentVoltages[i] = 0.0;
}

BatteryController::~BatteryController() {
}

void BatteryController::init() {
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);
}

float BatteryController::getVoltage() {
  double currentVoltage = 0.0;
  uint8_t vCount = 0;
  
  for (uint8_t i=0;i<CURRENT_VOLTAGES_BUFFER_SIZE;i++) {
    if (currentVoltages[i]==0) break;
    currentVoltage+=currentVoltages[i];
    vCount++;
  }

/*
  Serial.println(currentVoltage, DEC);
  Serial.println(vCount, DEC);
  Serial.println(currentVoltage / vCount, DEC);
  */
  
  return currentVoltage / vCount;
}

void BatteryController::update() {
  float vout = (analogRead(PIN_BATTERY_VOLTAGE) * 4.3) / 1024.0;
  float currentVoltage = vout / (R2/(R1+R2));

  //Serial.println(currentVoltage, 2);

  currentVoltages[currentVoltagesIndex] = currentVoltage;
  currentVoltagesIndex++;
  if (currentVoltagesIndex>=CURRENT_VOLTAGES_BUFFER_SIZE) currentVoltagesIndex = 0;

  BATT_STATE newState = BATT_INIT;

  if (isBatteryCritical()) {
    newState = BATT_CRITICAL;
  } else {
    newState = BATT_OK;
  }

  if (battState!=newState) {
    battState = newState;

    switch(battState) {
      case BATT_OK:
        Serial.println(F("USING BATT"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_OFF);
        break;
      case BATT_CRITICAL:
        Serial.println(F("BATT CRITICAL"));
        ledController->setState(INDEX_LED_BATT_STATE, LedController::LED_BLINK);
        break;
    }
  }
}

bool BatteryController::isBatteryCritical() {
  return getVoltage()<CRITICAL_BATTERY_VOLTAGE;
}

