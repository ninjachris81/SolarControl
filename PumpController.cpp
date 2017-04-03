#include "PumpController.h"

PumpController::PumpController(RelaisController* relaisController) : AbstractIntervalTask(UPDATE_PUMP_INTERVAL_MS) {
  this->relaisController = relaisController;
}

PumpController::~PumpController() {
}

void PumpController::init() {
  relaisController->setState(PIN_RELAIS_PUMP, false);
}

void PumpController::update2() {
  
}
