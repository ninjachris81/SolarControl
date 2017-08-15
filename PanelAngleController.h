#ifndef PANELANGLECONTROLLER_H
#define PANELANGLECONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <AbstractIntervalTask.h>

#include "BrightnessController.h"
#include "RelaisController.h"
#include "LedController.h"
#include "TimeController.h"
#include "Debug.h"

#define UPDATE_PA_INTERVAL_MS 2000

#ifdef IS_DEBUG
  #define LAST_STATES_MIN_COUNT 10
  #define LAST_STATES_LIMIT 15
  #define ADJUST_COUNTDOWN_SMALL 6.0
  #define ADJUST_COUNTDOWN_BIG 10.0
  #define INIT_SETUP_TIMEOUT 10.0
#else
  #define LAST_STATES_MIN_COUNT 180   // -> every 6 minutes
  #define LAST_STATES_LIMIT 200
  #define ADJUST_COUNTDOWN_SMALL 30.0
  #define ADJUST_COUNTDOWN_BIG 140.0
  #define INIT_SETUP_TIMEOUT 300.0 // 3 min down
#endif

//#define BRIGHTNESS_GLOBAL_THRESHOLD 30
#define UP_FACTOR 1.29       // up is slower -> need more time
#define END_PLUS 30       // for end states

class PanelAngleController : public AbstractIntervalTask {
public:
    enum ANGLE_STATE {
      AS_DEFAULT,
      AS_MIDDLE,
      AS_EAST,
      AS_MAX = AS_EAST,
      AS_OVERRIDE
    };

    PanelAngleController();
    virtual ~PanelAngleController();

    void init();
    
    void update();

    void setState(ANGLE_STATE state);

    void resetLastStates();

    ANGLE_STATE getState();

    int getMotorState();

    uint8_t getAdjustTimeoutProgress();

    void overrideMotorState(bool doEnable, bool directionUp);

private:
  void checkNewState();
  void checkState();
  void setMotorState(bool doEnable);
  void setMotorState(bool doEnable, bool directionUp);

  bool isMotorOverride = false;
  int currentMotorState = 0;

  unsigned long lastUpdate = 0;
  ANGLE_STATE currentState = AS_DEFAULT;
  uint8_t lastStates[4];
  uint8_t lastStatesCount = 0;
  
  bool isAdjustingAngle = true;
  uint16_t adjustCountdown = INIT_SETUP_TIMEOUT;
  uint16_t orgAdjustCountdown = INIT_SETUP_TIMEOUT;

};

#endif /* PANELANGLECONTROLLER_H */

