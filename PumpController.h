#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "RelaisController.h"
#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_PUMP_INTERVAL_MS 1000
  #define PUMP_STANDBY_INTERVAL_MS 6000
#else
  #define UPDATE_PUMP_INTERVAL_MS 10000
  #define PUMP_STANDBY_INTERVAL_MS 600000   // 10 min
#endif

class PumpController : public AbstractIntervalTask {
public:
    PumpController(RelaisController* relaisController);
    virtual ~PumpController();

    void init();
    
    void update2();

private:
  RelaisController* relaisController;

};

#endif /* PUMPCONTROLLER_H */

