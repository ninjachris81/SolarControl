#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "Pins.h"
#include <TimeLib.h>
#include <DCF77.h>

#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin

class TimeController : public AbstractIntervalTask {
public:
    TimeController();
    virtual ~TimeController();

    void init();
    
    void update2();

    bool isTimeSynced();

    uint8_t getHourOfDay();

private:
  bool timeSynced = false;
  DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);
};

#endif /* TIMECONTROLLER_H */

