#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "Pins.h"
#include "Debug.h"

#define LED_COUNT 3
#define BLINK_INTERVAL_MS 500

#define INDEX_LED_BATT_STATE 0
#define INDEX_LED_MOTOR_STATE 1
#define INDEX_LED_PUMP_STATE 2

class LedController : public AbstractIntervalTask {
public:
  enum LED_STATE {
    LED_OFF,
    LED_ON,
    LED_BLINK
  };

    LedController();
    virtual ~LedController();
    
    void setState(uint8_t index, LED_STATE state);

    void init();
    
    void update2();

    
private:
  int getPin(uint8_t index);
  LED_STATE ledStates[LED_COUNT];

};

#endif /* LEDCONTROLLER_H */

