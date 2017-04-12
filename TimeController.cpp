#include "TimeController.h"
#include "Debug.h"

TimeController::TimeController() : AbstractIntervalTask(1000) {
}

TimeController::~TimeController() {
}

void TimeController::init() {
  //DCF.Start();
}

void TimeController::update2() {
  /*
  time_t DCFtime = DCF.getTime(); // Check if new DCF77 time is available
  if (DCFtime!=0)
  {
    Serial.println(F("Time is updated"));
    setTime(DCFtime);
    setInterval(60000);
    timeSynced = true;
  }*/
}

bool TimeController::isTimeSynced() {
  return timeSynced;
}

uint8_t TimeController::getHourOfDay() {
  return hour();
}
