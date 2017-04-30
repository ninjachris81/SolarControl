#include "TimeController.h"
#include "Debug.h"
#include <LogHelper.h>

TimeController::TimeController() : AbstractIntervalTask(INIT_INTERVAL_MS) {
}

TimeController::~TimeController() {
}

void TimeController::init() {
  DCF.Start();
}

void TimeController::update() {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0)
  {
    LOG_PRINTLN(F("DCF77 Time set"));
    submitTime(DCFtime, true);
  } else {
    tmElements_t tm;
  
    if (RTC.read(tm)) {
      LOG_PRINTLN("RTC Time set");
      submitTime(makeTime(tm), false);
    } else {
      if (RTC.chipPresent()) {
        LOG_PRINTLN(F("The DS1307 stopped - setting default time"));
        tm.Hour = 14;
        tm.Minute = 0;
        tm.Second = 0;
        tm.Day = 15;
        tm.Month = 4;
        tm.Year = CalendarYrToTm(2017);
        if (RTC.write(tm)) {
          submitTime(makeTime(tm), false);
        } else {
          LOG_PRINTLN(F("DS1307 write error!"));
        }
      } else {
        LOG_PRINTLN(F("DS1307 read error!"));
      }
    }
  }
}

void TimeController::submitTime(time_t thisTime, bool fromDCF77) {
  LOG_PRINT(F("Submit time "));
  LOG_PRINT(fromDCF77 ? F("DCF ") : F("DS1307 "));
  LOG_PRINTLN(thisTime);
  
  setTime(thisTime);
  if (fromDCF77) {
    timeState = TIME_DCF77;

    // store in DS
    tmElements_t tm;
    breakTime(thisTime, tm);
    RTC.write(tm);
    setInterval(DCF_INTERVAL_MS);
  } else {
    if (timeState==TIME_INIT) timeState = TIME_DS1307;
    setInterval(DS_INTERVAL_MS);      // relax a bit - we have a time
  }
}

TimeController::TIME_STATE TimeController::getState() {
  return timeState;
}

uint8_t TimeController::getHourOfDay() {
  return hour();
}
