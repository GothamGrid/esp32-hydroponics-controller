// ShiftRegister.cpp
#include "ShiftRegister.hpp"

/**
 * @brief Constructs a new ShiftRegister object.
 * @param dataPin The GPIO pin number for serial data input (DS).
 * @param clockPin The GPIO pin number for shift register clock input (SHCP).
 * @param latchPin The GPIO pin number for storage register clock input (STCP).
 */
ShiftRegister::ShiftRegister(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin)
    : dataPin(dataPin), clockPin(clockPin), latchPin(latchPin), registers(0) {
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
}

/**
 * @brief Sets the state of an individual pin on the shift register.
 * @param pin The pin number (0-7) to set the state for.
 * @param state The state to set the pin to (HIGH or LOW).
 */
void ShiftRegister::setPinState(uint8_t pin, bool state) {
    if (state) {
        registers |= (1 << pin);
    } else {
        registers &= ~(1 << pin);
    }
}

/**
 * @brief Writes the current state to the shift register outputs.
 */
void ShiftRegister::write() {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, registers);
    digitalWrite(latchPin, HIGH);
}

/**
 * @brief Gets the current state of an individual pin on the shift register.
 * @param pin The pin number (0-7) to get the state for.
 * @return The current state of the specified pin (HIGH or LOW).
 */
bool ShiftRegister::getPinState(uint8_t pin) {
    return (registers >> pin) & 1;
}