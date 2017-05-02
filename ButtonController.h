#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <AbstractTask.h>

#include "Pins.h"
#include <FiveKeysHandler.h>

class ButtonController : public AbstractTask {
public:
    ButtonController();
    virtual ~ButtonController();

    void init();
    
    void update();

    void setButtonHandler(FiveKeysHandler::FKFeedbackHandler *handler);

private:
  FiveKeysHandler keysHandler = FiveKeysHandler(PIN_5BUTTONS);
};

#endif /* BUTTONCONTROLLER_H */

