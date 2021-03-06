#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#include "Pins.h"
#include <TimeLib.h>
#include <DCF77.h>
#include <Wire.h>
#include <DS1307RTC.h>

#define INIT_INTERVAL_MS 1000
#define DCF_INTERVAL_MS 60000

class TimeController : public AbstractIntervalTask {
public:
  enum TIME_STATE {
    TIME_INIT,
    TIME_DS1307,
    TIME_DCF77
  };

    TimeController();
    virtual ~TimeController();

    void init();
    
    void update();

    TIME_STATE getState();

    uint8_t getHourOfDay();

    bool hasDCF77Signal();

private:
  bool timeSynced = false;
  TIME_STATE timeState = TIME_INIT;
  
  DCF77 DCF = DCF77(PIN_DCF77,digitalPinToInterrupt(PIN_DCF77));

  void submitTime(time_t thisTime, bool fromDCF77);
};

#endif /* TIMECONTROLLER_H */

