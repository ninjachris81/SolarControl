#ifndef RELAISCONTROLLER_H
#define RELAISCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractTask.h"
#include "Pins.h"

class RelaisController : public AbstractTask {
public:
    RelaisController();
    virtual ~RelaisController();

    void init();
    
    void update();

    void setState(uint8_t pin, bool state);
    
private:

};

#endif /* RELAISCONTROLLER_H */

