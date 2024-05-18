/**
 * @file AppState.cpp
 * @brief Implementation of AppState class methods for managing system component states.
 */

#include "AppState.hpp"

/**
 * @brief Initializes state variables to false.
 */
AppState::AppState() 
    : powerOn(false), wifiLedDiodeState(false), pumpLedDiodeState(false),
      vegetableLedDiodeState(false), flowerLedDiodeState(false), 
      ledStripState(false), motorPumpState(false) {}

/**
 * @brief Sets the power state.
 * @param state The new state to set.
 */
void AppState::setPowerState(bool state) {
    powerOn = state;
}

/**
 * @brief Checks if the system is powered on.
 * @return True if powered on, otherwise false.
 */
bool AppState::isPowerOn() const {
    return powerOn;
}

/**
 * @brief Sets the state of the WiFi LED.
 * @param state The new state to set.
 */
void AppState::setWiFiLedDiodeState(bool state) {
    wifiLedDiodeState = state;
}

/**
 * @brief Checks if the WiFi LED is on.
 * @return True if on, otherwise false.
 */
bool AppState::isWiFiLedDiodeOn() const {
    return wifiLedDiodeState;
}

/**
 * @brief Sets the state of the pump LED.
 * @param state The new state to set.
 */
void AppState::setPumpLedDiodeState(bool state) {
    pumpLedDiodeState = state;
}

/**
 * @brief Checks if the pump LED is on.
 * @return True if on, otherwise false.
 */
bool AppState::isPumpLedDiodeOn() const {
    return pumpLedDiodeState;
}

/**
 * @brief Sets the state of the vegetable LED.
 * @param state The new state to set.
 */
void AppState::setVegetableLedDiodeState(bool state) {
    vegetableLedDiodeState = state;
}

/**
 * @brief Checks if the vegetable LED is on.
 * @return True if on, otherwise false.
 */
bool AppState::isVegetableLedDiodeOn() const {
    return vegetableLedDiodeState;
}

/**
 * @brief Sets the state of the flower LED.
 * @param state The new state to set.
 */
void AppState::setFlowerLedDiodeState(bool state) {
    flowerLedDiodeState = state;
}

/**
 * @brief Checks if the flower LED is on.
 * @return True if on, otherwise false.
 */
bool AppState::isFlowerLedDiodeOn() const {
    return flowerLedDiodeState;
}

/**
 * @brief Sets the state of a specific LED diode.
 * @param ledDiode The specific diode to modify.
 * @param state The new state to set.
 */
void AppState::setLedDiodeState(DiodeType ledDiode, bool state) {
    switch (ledDiode) {
        case DiodeType::Power:
            powerOn = state;
            break;
        case DiodeType::WiFi:
            wifiLedDiodeState = state;
            break;
        case DiodeType::Pump:
            pumpLedDiodeState = state;
            break;
        case DiodeType::Vegetable:
            vegetableLedDiodeState = state;
            break;
        case DiodeType::Flower:
            flowerLedDiodeState = state;
            break;
    }
}

/**
 * @brief Retrieves the current state of a specific LED diode.
 * @param ledDiode The diode for which to get the state.
 * @return The current state of the specified diode.
 */
bool AppState::getStateForLedDiode(DiodeType ledDiode) const {
    switch (ledDiode) {
        case DiodeType::Power: return powerOn;
        case DiodeType::WiFi: return wifiLedDiodeState;
        case DiodeType::Pump: return pumpLedDiodeState;
        case DiodeType::Vegetable: return vegetableLedDiodeState;
        case DiodeType::Flower: return flowerLedDiodeState;
        default: return false;
    }
}

/**
 * @brief Sets the state of the LED strip.
 * @param state The new state to set.
 */
void AppState::setLedStripState(bool state) {
    ledStripState = state;
}

/**
 * @brief Checks if the LED strip is on.
 * @return True if on, otherwise false.
 */
bool AppState::isLedStripOn() const {
    return ledStripState;
}

/**
 * @brief Sets the state of the motor pump.
 * @param state The new state to set.
 */
void AppState::setMotorPumpState(bool state) {
    motorPumpState = state;
}

/**
 * @brief Retrieves the current state of the motor pump.
 * @return The current state of the motor pump.
 */
bool AppState::getMotorPumpState() const {
    return motorPumpState;
}
