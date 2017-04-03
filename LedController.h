#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "Pins.h"

#define LED_COUNT 2
#define BLINK_INTERVAL_MS 400

class LedController : public AbstractIntervalTask {
public:
    LedController();
    virtual ~LedController();
    
    void setBlinking(uint8_t index, bool isBlinking);

    void setState(uint8_t index, bool isOn);

    void init();
    
    void update2();
private:
    bool blinkingLeds[LED_COUNT];

};

#endif /* LEDCONTROLLER_H */

