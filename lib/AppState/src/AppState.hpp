/**
 * @file AppState.hpp
 * @brief Manages the state of the system components in a smart gardening system.
 */

#ifndef AppState_hpp
#define AppState_hpp

#include "LEDController.hpp"

/**
 * @class AppState
 * @brief Holds and manages states for various system components.
 */
class AppState {
public:
    /**
     * @brief Constructor for AppState. Initializes all states to false.
     */
    AppState();
    
    // State checkers

    /**
     * @brief Checks if the system power is on.
     * @return True if power is on, false otherwise.
     */
    bool isPowerOn() const;

    /**
     * @brief Checks if the WiFi LED diode is on.
     * @return True if the WiFi LED is on, false otherwise.
     */
    bool isWiFiLedDiodeOn() const;

    /**
     * @brief Checks if the pump LED diode is on.
     * @return True if the pump LED is on, false otherwise.
     */
    bool isPumpLedDiodeOn() const;

    /**
     * @brief Checks if the vegetable LED diode is on.
     * @return True if the vegetable LED is on, false otherwise.
     */
    bool isVegetableLedDiodeOn() const;

    /**
     * @brief Checks if the flower LED diode is on.
     * @return True if the flower LED is on, false otherwise.
     */
    bool isFlowerLedDiodeOn() const;

    /**
     * @brief Checks if the LED strip is on.
     * @return True if the LED strip is on, false otherwise.
     */
    bool isLedStripOn() const;

    // State setters

    /**
     * @brief Sets the power state.
     * @param state New state of the power.
     */
    void setPowerState(bool state);

    /**
     * @brief Sets the state of the WiFi LED diode.
     * @param state New state of the WiFi LED diode.
     */
    void setWiFiLedDiodeState(bool state);

    /**
     * @brief Sets the state of the pump LED diode.
     * @param state New state of the pump LED diode.
     */
    void setPumpLedDiodeState(bool state);

    /**
     * @brief Sets the state of the vegetable LED diode.
     * @param state New state of the vegetable LED diode.
     */
    void setVegetableLedDiodeState(bool state);

    /**
     * @brief Sets the state of the flower LED diode.
     * @param state New state of the flower LED diode.
     */
    void setFlowerLedDiodeState(bool state);

    /**
     * @brief Sets the state of the LED strip.
     * @param state New state of the LED strip.
     */
    void setLedStripState(bool state);

    /**
     * @brief Sets the state of a specified LED diode.
     * @param diode Type of diode to set.
     * @param state New state of the specified diode.
     */
    void setLedDiodeState(DiodeType diode, bool state);

    /**
     * @brief Retrieves the state for a specified LED diode.
     * @param diode Type of diode to query.
     * @return Current state of the specified diode.
     */
    bool getStateForLedDiode(DiodeType diode) const;

private:
    bool powerOn;
    bool wifiLedDiodeState;
    bool pumpLedDiodeState;
    bool vegetableLedDiodeState;
    bool flowerLedDiodeState;
    bool ledStripState;
};

#endif /* AppState_hpp */
