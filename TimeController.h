#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractTask.h"
#include "Pins.h"

class TimeController : public AbstractTask {
public:
    TimeController();
    virtual ~TimeController();

    void init();
    
    void update();

private:

};

#endif /* TIMECONTROLLER_H */

