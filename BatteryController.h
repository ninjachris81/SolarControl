#ifndef BATTERYCONTROLLER_H
#define BATTERYCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#include "LedController.h"
#include "RelaisController.h"
#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_BATTERY_INTERVAL_MS 1000
#else
  #define UPDATE_BATTERY_INTERVAL_MS 500
#endif

#define CURRENT_VOLTAGES_BUFFER_SIZE 10

//#define R1 30000.0
//#define R2 7500.0

#define R1 29700.0
#define R2 7430.0

#define CRITICAL_BATTERY_VOLTAGE 11.0

class BatteryController : public AbstractIntervalTask {
public:
    BatteryController(LedController* ledController, RelaisController* relaisController);
    virtual ~BatteryController();

  enum BATT_STATE {
    BATT_INIT,
    BATT_OK,
    BATT_CRITICAL
  };

    void init();
    
    void update();

    bool isBatteryCritical();

    float getVoltage();

private:
  BATT_STATE battState = BATT_INIT;
  float currentVoltages[CURRENT_VOLTAGES_BUFFER_SIZE];
  uint8_t currentVoltagesIndex = 0;

  LedController* ledController;
  RelaisController* relaisController;
};

#endif /* BATTERYCONTROLLER_H */

