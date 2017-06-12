#ifndef REMOTELOGGER
#define REMOTELOGGER

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Debug.h"

#include <AbstractIntervalTask.h>

#ifdef IS_DEBUG
  #define LOGGER_INTERVAL_MS 2000
#else
  #define LOGGER_INTERVAL_MS 10000
#endif


class RemoteLogger : public AbstractIntervalTask {
public:
  RemoteLogger();

  void init();

  void update();

private:
  
};


#endif // REMOTELOGGER
