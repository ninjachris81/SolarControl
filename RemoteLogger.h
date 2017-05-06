#ifndef REMOTELOGGER
#define REMOTELOGGER

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#define LOGGER_INTERVAL_MS 10000

class RemoteLogger : public AbstractIntervalTask {
public:
  RemoteLogger();

  void init();

  void update();

private:
  
};


#endif // REMOTELOGGER
