/**
 * @file PumpController.cpp
 * @brief Implementation of PumpController class methods for managing the motor pump.
 */

#include "PumpController.hpp"
#include "DebugLogger.hpp"

/**
 * @brief Constructor for PumpController. Initializes the shift register reference.
 * @param shiftReg Reference to the ShiftRegister instance.
 */
PumpController::PumpController(ShiftRegister& shiftReg)
    : shiftRegister(shiftReg), motorPumpState(false) {}

/**
 * @brief Sets the state of the motor pump.
 * @param state The new state to set.
 */
void PumpController::setMotorPumpState(bool state) {
    motorPumpState = state;
    shiftRegister.setPinState(2, state); // Use setPinState to control the motor pump
    shiftRegister.write(); // Ensure the state is written to the shift register

    // Log the current state of the motor pump and pin 2
    DebugLogger::info("Motor pump state set to: " + String(state ? "ON" : "OFF"));
    DebugLogger::info("Pin 2 state: " + String(shiftRegister.getPinState(2) ? "HIGH" : "LOW"));
}

/**
 * @brief Retrieves the current state of the motor pump.
 * @return The current state of the motor pump.
 */
bool PumpController::getMotorPumpState() const {
    DebugLogger::info("Current motor pump state: " + String(motorPumpState ? "ON" : "OFF"));
    return motorPumpState;
}
