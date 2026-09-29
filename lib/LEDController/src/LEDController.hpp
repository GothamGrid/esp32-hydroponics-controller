#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <Arduino.h>
#include "ShiftRegister.hpp"
#include "DiodeTypes.hpp"

// ★ REMOVED: #include "WiFiManager.hpp"
// LEDController controls LEDs; it no longer manages the WiFi connection.

/**
 * LEDController manages the LED diodes and LED strip,
 * including their colors and states.
 */
class LEDController {
public:
    /**
     * Constructor for LEDController.
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

    /*
     * ★ REMOVED:
     *
     * void setWiFiManager(WiFiManager& manager);
     * void updateWiFiLedDiodeStatus(bool isConnected);
     * void blinkWiFiLedDiode(int count = 1);
     *
     * These functions mixed WiFi connection management with physical
     * LED control and could not guarantee a complete blink sequence.
     */

    // ★ Starts a complete, non-blocking WiFi blink sequence.
    void startWiFiBlink(uint8_t count = 1);

    // ★ Processes an active blink sequence.
    void update();

    // ★ Reports whether the sequencer currently owns the LED.
    bool isWiFiBlinking() const;

    // ★ Immediately stops blinking and turns the WiFi LED off.
    void cancelWiFiBlink();

    /**
     * Sets the specified LED diode to the desired state.
     */
    void setLedDiodeState(
        DiodeType ledDiode,
        bool ledDiodeState
    );

    /**
     * Toggles the state of a specified LED diode.
     */
    void toggleLedDiodeState(DiodeType ledDiode);

    /**
     * Configures the LED strip based on its mode.
     */
    void setLedStripMode(uint8_t ledStripMode);

    /**
     * Recursively sets multiple LED states.
     */
    template<typename... Args>
    void tuneMultipleLedAttributes(
        DiodeType ledDiode,
        bool ledDiodeState,
        Args... rest
    ) {
        setLedDiodeState(ledDiode, ledDiodeState);
        tuneMultipleLedAttributes(rest...);
    }

    /**
     * Terminates the recursive operation.
     */
    void tuneMultipleLedAttributes() {}

private:
    ShiftRegister* shiftRegister;

    uint8_t powerLedDiodePin;
    uint8_t wifiLedDiodePin;
    uint8_t pumpLedDiodePin;
    uint8_t vegetableLedDiodePin;
    uint8_t flowerLedDiodePin;

    uint8_t bluePWMPin;
    uint8_t redPWMPin;
    uint8_t greenPWMPin;

    // Complete non-blocking blink-sequencer state.
    bool wifiBlinkActive;
    bool wifiBlinkState;
    uint16_t wifiBlinkTransitionsRemaining;
    unsigned long lastWiFiBlinkMillis;

    // Each ON or OFF phase lasts 200 ms.
    const unsigned long wifiBlinkInterval = 200;

    uint8_t getLedDiodePin(DiodeType diode) const;
};

#endif // LED_CONTROLLER_HPP