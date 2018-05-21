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

#define BRIGHTNESS_BRIGHT_LEVEL 15.0

#define BRIGHTNESS_BRIGHT_LEVEL_MIN 10.0
#define BRIGHTNESS_BRIGHT_LEVEL_MAX 35.0

#define BRIGHTNESS_BRIGHT_LEVEL_DAY_ADD 120.0

#define ADJUST_DELTA_UP 0.1
#define ADJUST_DELTA_DOWN 0.005

class BrightnessController : public AbstractIntervalTask {
public:
    BrightnessController();
    virtual ~BrightnessController();

    void init();
    
    void update();

    int getSensorValue();

    bool isBright();

    bool isDay();

    void adjustLevels(bool brighter);

    float getBrightLevel();

private:
  SmoothValue sensorValue = SmoothValue(20, -1.0);

  float brightLevel;

};

#endif /* BRIGHTNESSCONTROLLER_H */

