#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#include "RelaisController.h"
#include "LedController.h"
#include "BatteryController.h"
#include "BrightnessController.h"
#include "TimeController.h"
#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_PUMP_INTERVAL_MS 1000
  #define PUMP_STANDBY_INTERVAL_ON_MS 5000
  #define PUMP_STANDBY_INTERVAL_OFF_MS 10000
  #define PUMP_TIMEOUT 3
#else
  #define UPDATE_PUMP_INTERVAL_MS 10000
  #define PUMP_STANDBY_INTERVAL_ON_MS 60000   // 1 min
  #define PUMP_STANDBY_INTERVAL_OFF_MS 1200000   // 20 min
  #define PUMP_TIMEOUT 6
#endif

class PumpController : public AbstractIntervalTask {
public:
    PumpController(RelaisController* relaisController, LedController* ledController, BatteryController* batteryController, BrightnessController* brightnessController, TimeController* timeController);
    virtual ~PumpController();

    void init();
    
    void update();

    void setState(bool pumpOn);

    bool getState();

    void overrideState(bool isOverride, bool pumpOn);

    bool hasOverride();

private:
  bool isOverride = false;

  bool pumpOn = false;
  unsigned long lastToggle = 0;
  
  RelaisController* relaisController;
  LedController* ledController;
  BatteryController* batteryController;
  BrightnessController* brightnessController;
  TimeController* timeController;

};

#endif /* PUMPCONTROLLER_H */

