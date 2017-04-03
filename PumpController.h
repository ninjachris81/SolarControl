#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "RelaisController.h"
#include "LedController.h"
#include "BatteryController.h"
#include "BrightnessController.h"
#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_PUMP_INTERVAL_MS 1000
  #define PUMP_STANDBY_INTERVAL_BATTERY_MS 6000
  #define PUMP_STANDBY_INTERVAL_AC_MS 12000
#else
  #define UPDATE_PUMP_INTERVAL_MS 10000
  #define PUMP_STANDBY_INTERVAL_BATTERY_MS 600000   // 10 min
  #define PUMP_STANDBY_INTERVAL_AC_MS 1200000        // 20 min
#endif

class PumpController : public AbstractIntervalTask {
public:
    PumpController(RelaisController* relaisController, LedController* ledController, BatteryController* batteryController, BrightnessController* brightnessController);
    virtual ~PumpController();

    void init();
    
    void update2();

    void setState(bool pumpOn);

private:
  bool pumpOn = false;
  
  RelaisController* relaisController;
  LedController* ledController;
  BatteryController* batteryController;
  BrightnessController* brightnessController;

};

#endif /* PUMPCONTROLLER_H */

