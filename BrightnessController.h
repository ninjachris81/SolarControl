#ifndef BRIGHTNESSCONTROLLER_H
#define BRIGHTNESSCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>
#include <SmoothValue.h>

#include "Pins.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_BRIGHTNESS_INTERVAL_MS 1000
#else
  #define UPDATE_BRIGHTNESS_INTERVAL_MS 5000
#endif

#define BRIGHTNESS_DARK_LEVEL 50

class BrightnessController : public AbstractIntervalTask {
public:
    BrightnessController();
    virtual ~BrightnessController();

    void init();
    
    void update();

    int getSensorValue();

    bool isDark();

private:
  SmoothValue sensorValue;

};

#endif /* BRIGHTNESSCONTROLLER_H */

