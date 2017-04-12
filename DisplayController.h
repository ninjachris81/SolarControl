#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <LedControl.h>
#include "AbstractIntervalTask.h"
#include "ButtonController.h"
#include "BrightnessController.h"
#include "BatteryController.h"
#include "PanelAngleController.h"
#include "PumpController.h"
#include "TimeController.h"

#include "Debug.h"
#include "Pins.h"

#define DISPLAY_BRIGHTNESS 4

#ifdef IS_DEBUG
  #define DISPLAY_TIMEOUT_MS 20
#else
  #define DISPLAY_TIMEOUT_MS 120
#endif

#define DEFAULT_DISPLAY_ADDR 0


class DisplayController : public AbstractIntervalTask, public JoystickHandler::JoystickFeedbackHandler {
public:
  enum DISPLAY_CONTENT {
    DC_TIME,
    DC_BRIGHTNESS,
    DC_BATTERY,
    DC_PANEL_ANGLE,
    DC_PUMP,
    DC_MAX = DC_PUMP,
    DC_MIN = DC_TIME
  };

    DisplayController(ButtonController* buttonController, BrightnessController* brightnessController, BatteryController* batteryController, PanelAngleController* panelAngleController, PumpController* pumpController, TimeController* timeController);
    virtual ~DisplayController();

    void init();
    
    void update2();

    void onLeft(bool isDown);
    void onRight(bool isDown);
    void onUp(bool isDown);
    void onDown(bool isDown);
    void onPressed(bool isDown);
    
private:
  ButtonController* buttonController;
  BrightnessController* brightnessController;
  BatteryController* batteryController;
  PanelAngleController* panelAngleController;
  PumpController* pumpController;
  TimeController* timeController;

  void printNumber(int addr, int v);
  void printNumber(int addr, float v);
  void printBool(int addr, bool v, uint8_t offset);

  bool displayOn = true;
  int displayContent = DC_TIME;
  uint16_t displayTimeout = DISPLAY_TIMEOUT_MS;

  LedControl ledControl = LedControl(PIN_DISPLAY_DIN,PIN_DISPLAY_CLK,PIN_DISPLAY_CS,1);

  void setDisplayOn(bool newDisplayOn);
  void updateDisplay();
};

#endif /* DISPLAYCONTROLLER_H */

