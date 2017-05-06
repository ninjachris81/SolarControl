#ifndef BATTERYCONTROLLER_H
#define BATTERYCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>
#include <SmoothValue.h>

#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_BATTERY_INTERVAL_MS 1000
#else
  #define UPDATE_BATTERY_INTERVAL_MS 500
#endif

#define VOLTAGE_CORRECTION 2.0

//#define R1 30000.0
//#define R2 7500.0

#define R1 29700.0
#define R2 7430.0

#define CRITICAL_BATTERY_VOLTAGE 11.5

class BatteryController : public AbstractIntervalTask {
public:
    BatteryController();
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
  SmoothValue currentVoltage;
  BATT_STATE battState = BATT_INIT;
};

#endif /* BATTERYCONTROLLER_H */

