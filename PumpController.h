#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_PUMP_INTERVAL_MS 1000
  #define PUMP_STANDBY_INTERVAL_ON_MIN_MS 6000
  #define PUMP_STANDBY_INTERVAL_ON_MAX_MS 24000
  #define PUMP_STANDBY_INTERVAL_OFF_MS 10000
  #define PUMP_TIMEOUT 3
  #define PUMP_STANDBY_DELTA_CHANGE 100
  #define BATTERY_FULL_TIMEOUT 3
#else
  #define UPDATE_PUMP_INTERVAL_MS 10000
  #define PUMP_STANDBY_INTERVAL_ON_MIN_MS 60000   // 1 min
  #define PUMP_STANDBY_INTERVAL_ON_MAX_MS 240000   // 4 min
  #define PUMP_STANDBY_INTERVAL_OFF_MS 1200000   // 20 min
  #define PUMP_TIMEOUT 6
  #define PUMP_STANDBY_DELTA_CHANGE 1000      // 30 minutes MAX<->MIN
  #define BATTERY_FULL_TIMEOUT 6
#endif

class PumpController : public AbstractIntervalTask {
public:
    PumpController();
    virtual ~PumpController();

    void init();
    
    void update();

    void setState(bool pumpOn);

    bool getState();

    void overrideState(bool isOverride, bool pumpOn);

    bool hasOverride();

    int getRemainingMinutes();

private:
  bool isOverride = false;
  unsigned long currentStandbyOnIntervalMs = PUMP_STANDBY_INTERVAL_ON_MIN_MS;
  uint8_t batteryFullTimeout = 0;

  bool pumpOn = false;
  unsigned long lastToggle = 0;
};

#endif /* PUMPCONTROLLER_H */

