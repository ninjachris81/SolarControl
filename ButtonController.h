#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "AbstractTask.h"
#include "Pins.h"
#include "joystick_handler.h"

class ButtonController : public AbstractTask {
public:
    ButtonController();
    virtual ~ButtonController();

    void init();
    
    void update();

    void setJoystickHandler(JoystickHandler::JoystickFeedbackHandler *handler);

private:
  JoystickHandler joystick = JoystickHandler(PIN_BUTTON_X, PIN_BUTTON_Y, PIN_BUTTON_BTN);
};

#endif /* BUTTONCONTROLLER_H */

