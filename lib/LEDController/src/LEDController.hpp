#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <Arduino.h>
#include "WiFiManager.hpp"
#include "ShiftRegister.hpp"
#include "DiodeTypes.hpp"

/**
 * LEDController manages the LED diodes and LED strip, including their colors and states.
 */
class LEDController {
public:
    /**
     * Constructor for LEDController.
     * Initializes with specific pins and components.
     */
    LEDController(
        ShiftRegister* shiftRegister, 
        uint8_t powerLedDiodePin, 
        uint8_t wifiLedDiodePin, 
        uint8_t pumpLedDiodePin, 
        uint8_t vegetableLedDiodePin, 
        uint8_t flowerLedDiodePin, 
        uint8_t bluePWMPin, 
        uint8_t redPWMPin, 
        uint8_t greenPWMPin
    );

    /**
     * Associates a WiFiManager to enable LED feedback based on WiFi state.
     */
    void setWiFiManager(WiFiManager& manager);

    /**
     * Updates the LED indicator for WiFi connectivity.
     */
    void updateWiFiLedDiodeStatus(bool isConnected);

    /**
     * Triggers a blinking effect on the WiFi LED.
     */
    void blinkWiFiLedDiode(int count = 1);

    /**
     * Sets the specified LED diode to the desired state (on/off).
     */
    void setLedDiodeState(DiodeType ledDiode, bool ledDiodeState);

    /**
     * Toggles the state of a specified LED diode.
     */
    void toggleLedDiodeState(DiodeType ledDiode);

    /**
     * Configures the LED strip color or turns it off based on the mode.
     */
    void setLedStripMode(uint8_t ledStripMode);

    /**
     * Recursively sets the state of multiple LEDs.
     */
    template<typename... Args>
    void tuneMultipleLedAttributes(DiodeType ledDiode, bool ledDiodeState, Args... rest) {
        setLedDiodeState(ledDiode, ledDiodeState);
        tuneMultipleLedAttributes(rest...);
    }

    /**
     * Terminates the recursion for setting multiple LED states.
     */
    void tuneMultipleLedAttributes() {}
    
private:
    ShiftRegister* shiftRegister; // Manages shift register for LED control
    uint8_t powerLedDiodePin, wifiLedDiodePin, pumpLedDiodePin, vegetableLedDiodePin, flowerLedDiodePin; // Pin numbers for each LED diode
    uint8_t bluePWMPin, redPWMPin, greenPWMPin; // PWM pins for LED strip colors
    WiFiManager* wifiManager; // Pointer to the WiFiManager for network status
    bool ledBlinkState; // Current state of LED blinking (on/off)
    unsigned long lastBlinkMillis; // Timestamp of the last LED blink
    const unsigned long blinkInterval = 500; // Interval between blinks
    int wifiBlinkCounter; // Counter for blinking WiFi LED
    uint8_t getLedDiodePin(DiodeType diode) const; // Returns the pin number for a given diode type
};

#endif // LED_CONTROLLER_HPP
