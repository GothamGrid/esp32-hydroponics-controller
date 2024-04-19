// ButtonManager.cpp
#include "ButtonManager.hpp"
#include "DebugLogger.hpp"

/**
 * @brief Constructs a new ButtonManager object.
 * @param pin The GPIO pin number for the button.
 */
ButtonManager::ButtonManager(uint8_t pin)
    : pin(pin), lastButtonState(HIGH), lastDebounceTime(0) {}

/**
 * @brief Sets up the button pin as an input with a pull-up resistor.
 */
void ButtonManager::setup() {
    pinMode(pin, INPUT_PULLUP);
    DebugLogger::info("Button initialized on pin " + String(pin));
}

/**
 * @brief Checks if the button has been clicked (short press).
 * @return True if the button has been clicked, false otherwise.
 */
bool ButtonManager::isClicked() {
    bool currentState = digitalRead(pin);
    bool clicked = (currentState == LOW && lastButtonState == HIGH);
    lastButtonState = currentState;
    if (clicked) {
        DebugLogger::info("Button clicked on pin " + String(pin));
    }
    return clicked;
}

/**
 * @brief Updates the button state based on the current input and debounce delay.
 *
 * This method should be called regularly (e.g., in the loop() function) to
 * keep the button state updated.
 */
void ButtonManager::update() {
    bool currentState = digitalRead(pin);
    if (currentState != lastButtonState) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
        lastButtonState = currentState;
    }
}