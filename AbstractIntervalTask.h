#ifndef ABSTRACTINTERVALTASK_H
#define ABSTRACTINTERVALTASK_H

#include "AbstractTask.h"

class AbstractIntervalTask : public AbstractTask {
public:
  AbstractIntervalTask(unsigned long intervalMs) {
    this->intervalMs = intervalMs;
  }

  void update() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= intervalMs) {      
      lastUpdate = currentMillis;
      update2();
    }
  };

protected:
  void setInterval(unsigned long intervalMs) {
    this->intervalMs = intervalMs;
  }

  virtual void update2() = 0;

private:
  unsigned long intervalMs = 1000;
  unsigned long lastUpdate;
};

#endif /* ABSTRACTINTERVALTASK_H */
