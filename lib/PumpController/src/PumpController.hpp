/**
 * @file PumpController.hpp
 * @brief Manages the motor pump in a smart gardening system.
 */

#ifndef PUMPCONTROLLER_HPP
#define PUMPCONTROLLER_HPP

#include "ShiftRegister.hpp"
#include "AppState.hpp"

/**
 * @class PumpController
 * @brief Controls the motor pump via the shift register.
 */
class PumpController {
public:
    /**
     * @brief Constructor for PumpController.
     * @param shiftReg Reference to the ShiftRegister instance.
     */
    PumpController(ShiftRegister& shiftReg);

    /**
     * @brief Sets the state of the motor pump.
     * @param state The new state to set.
     */
    void setMotorPumpState(bool state);

    /**
     * @brief Retrieves the current state of the motor pump.
     * @return The current state of the motor pump.
     */
    bool getMotorPumpState() const;

private:
    ShiftRegister& shiftRegister; ///< Reference to the shift register
    bool motorPumpState; ///< Current state of the motor pump
};

#endif // PUMPCONTROLLER_HPP
