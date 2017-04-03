#ifndef ABSTRACTINTERVALTASK_H
#define ABSTRACTINTERVALTASK_H

#include "AbstractTask.h"

class AbstractIntervalTask : public AbstractTask {
public:
  AbstractIntervalTask(uint16_t intervalMs) {
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
  void setInterval(uint16_t intervalMs) {
    this->intervalMs = intervalMs;
  }

  virtual void update2() = 0;

private:
  uint16_t intervalMs = 1000;
  unsigned long lastUpdate;
};

#endif /* ABSTRACTINTERVALTASK_H */
