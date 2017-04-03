#ifndef PANELANGLECONTROLLER_H
#define PANELANGLECONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AbstractIntervalTask.h"
#include "BrightnessController.h"
#include "RelaisController.h"
#include "LedController.h"
#include "Debug.h"

#define UPDATE_PA_INTERVAL_MS 1000

#ifdef IS_DEBUG
  #define LAST_STATES_MIN_COUNT 10
  #define ADJUST_COUNTDOWN 6
#else
  #define LAST_STATES_MIN_COUNT 180   // -> every 3 minutes
  #define ADJUST_COUNTDOWN 60
#endif

#define BRIGHTNESS_GLOBAL_THRESHOLD 100
#define BRIGHTNESS_DELTA_EAST -20
#define BRIGHTNESS_DELTA_WEST 20

class PanelAngleController : public AbstractIntervalTask {
public:
    enum ANGLE_STATE {
      AS_DEFAULT,
      AS_MIDDLE,
      AS_EAST,
      AS_MAX = AS_EAST
    };

    PanelAngleController(BrightnessController* brightnessController, RelaisController* relaisController, LedController* ledController);
    virtual ~PanelAngleController();

    void init();
    
    void update2();

    void setState(ANGLE_STATE state);

    void resetLastStates();

private:
  void checkNewState();
  void checkState();
  void setMotorState(bool doEnable);
  void setMotorState(bool doEnable, bool directionUp);

  unsigned long lastUpdate = 0;
  ANGLE_STATE currentState = AS_DEFAULT;
  uint8_t lastStates[4];
  bool isAdjustingAngle = false;
  uint8_t adjustCountdown = 0;
  
  BrightnessController* brightnessController;
  RelaisController* relaisController;
  LedController* ledController;

};

#endif /* PANELANGLECONTROLLER_H */

