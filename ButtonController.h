#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractTask.h"
#include "Pins.h"

class ButtonController : public AbstractTask {
public:
    ButtonController();
    virtual ~ButtonController();

    void init();
    
    void update();

    bool consumeButtonClick();

private:
  bool buttonClicked = false;

};

#endif /* BUTTONCONTROLLER_H */

