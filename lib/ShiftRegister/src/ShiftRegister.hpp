// ShiftRegister.h
#ifndef ShiftRegister_h
#define ShiftRegister_h

#include <Arduino.h>

/**
 * @brief Controls a 74HC595N shift register.
 */
class ShiftRegister {
public:
    /**
     * @brief Constructs a new ShiftRegister object.
     * @param dataPin The GPIO pin number for serial data input (DS).
     * @param clockPin The GPIO pin number for shift register clock input (SHCP).
     * @param latchPin The GPIO pin number for storage register clock input (STCP).
     */
    ShiftRegister(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);

    /**
     * @brief Sets the state of an individual pin on the shift register.
     * @param pin The pin number (0-7) to set the state for.
     * @param state The state to set the pin to (HIGH or LOW).
     */
    void setPinState(uint8_t pin, bool state);

    /**
     * @brief Writes the current state to the shift register outputs.
     */
    void write();

    /**
     * @brief Gets the current state of an individual pin on the shift register.
     * @param pin The pin number (0-7) to get the state for.
     * @return The current state of the specified pin (HIGH or LOW).
     */
    bool getPinState(uint8_t pin);

private:
    uint8_t dataPin;   // The GPIO pin number for serial data input (DS).
    uint8_t clockPin;  // The GPIO pin number for shift register clock input (SHCP).
    uint8_t latchPin;  // The GPIO pin number for storage register clock input (STCP).
    uint8_t registers; // The current state of the shift register.
};

#endif