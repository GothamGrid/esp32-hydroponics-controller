// ButtonManager.h
#ifndef ButtonManager_h
#define ButtonManager_h

#include <Arduino.h>
#include "DebugLogger.hpp"

/**
 * @brief Manages button input with debouncing and click detection.
 */
class ButtonManager {
public:
    /**
     * @brief Constructs a new ButtonManager object.
     * @param pin The GPIO pin number for the button.
     */
    ButtonManager(uint8_t pin);

    /**
     * @brief Sets up the button pin as an input with a pull-up resistor.
     */
    void setup();

    /**
     * @brief Checks if the button has been clicked (short press).
     * @return True if the button has been clicked, false otherwise.
     */
    bool isClicked();

    /**
     * @brief Updates the button state based on the current input and debounce delay.
     *
     * This method should be called regularly (e.g., in the loop() function) to
     * keep the button state updated.
     */
    void update();

private:
    uint8_t pin; // GPIO pin number associated with the button
    bool lastButtonState; // The previous read state of the button
    unsigned long lastDebounceTime; // Last time the button state changed
    static constexpr unsigned long debounceDelay = 80; // Debounce delay in milliseconds   
};

#endif