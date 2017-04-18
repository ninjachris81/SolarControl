#include "TimeController.h"
#include "Debug.h"

TimeController::TimeController() : AbstractIntervalTask(1000) {
}

TimeController::~TimeController() {
}

void TimeController::init() {
  DCF.Start();
}

void TimeController::update2() {
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0)
  {
    Serial.println(F("DCF77 Time set"));
    submitTime(DCFtime, true);
  } else {
    tmElements_t tm;
  
    if (RTC.read(tm)) {
      Serial.print("RTC Time set");
      submitTime(makeTime(tm), false);
    } else {
      if (RTC.chipPresent()) {
        Serial.println(F("The DS1307 stopped - setting default time"));
        tm.Hour = 14;
        tm.Minute = 0;
        tm.Second = 0;
        tm.Day = 15;
        tm.Month = 4;
        tm.Year = CalendarYrToTm(2017);
        if (RTC.write(tm)) {
          submitTime(makeTime(tm), false);
        } else {
          Serial.println(F("DS1307 write error!"));
        }
      } else {
        Serial.println(F("DS1307 read error!"));
      }
    }
  }
}

void TimeController::submitTime(time_t thisTime, bool fromDCF77) {
  Serial.print(F("Submit time "));
  Serial.print(fromDCF77 ? F("DCF ") : F("DS1307 "));
  Serial.println(thisTime);
  
  setTime(thisTime);
  if (fromDCF77) {
    timeState = TIME_DCF77;

    // store in DS
    tmElements_t tm;
    breakTime(thisTime, tm);
    RTC.write(tm);
    setInterval(60000);
  } else {
    timeState = TIME_DS1307;
    setInterval(1000);
  }
}

TimeController::TIME_STATE TimeController::getState() {
  return timeState;
}

uint8_t TimeController::getHourOfDay() {
  return hour();
}
