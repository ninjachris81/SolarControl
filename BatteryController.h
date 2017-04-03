#ifndef BATTERYCONTROLLER_H
#define BATTERYCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "Pins.h"
#include "Debug.h"

#define UPDATE_BATTERY_INTERVAL_MS 3000

class BatteryController : public AbstractIntervalTask {
public:
    BatteryController();
    virtual ~BatteryController();

    void init();
    
    void update2();

};

#endif /* BATTERYCONTROLLER_H */

