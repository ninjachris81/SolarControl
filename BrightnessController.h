#ifndef BRIGHTNESSCONTROLLER_H
#define BRIGHTNESSCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Pins.h"
#include "AbstractIntervalTask.h"
#include "Debug.h"

#ifdef IS_DEBUG
  #define UPDATE_BRIGHTNESS_INTERVAL_MS 1000
#else
  #define UPDATE_BRIGHTNESS_INTERVAL_MS 5000
#endif

#define BRIGHTNESS_DARK_LEVEL 500

class BrightnessController : public AbstractIntervalTask {
public:
    BrightnessController();
    virtual ~BrightnessController();

    void init();
    
    void update2();

    int getSensorValueEast();
    int getSensorValueWest();

    bool isDark();
    
private:
  int sensorValueEast = 0;
  int sensorValueWest = 0;

};

#endif /* BRIGHTNESSCONTROLLER_H */

