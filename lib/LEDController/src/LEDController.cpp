// LEDController.cpp
#include "LEDController.hpp"
#include "DebugLogger.hpp"
#include "WiFiManager.hpp"

/**
 * Constructs a LEDController to manage LED diodes and strips.
 * 
 * @param shiftRegister Pointer to a ShiftRegister object for controlling LEDs via a shift register.
 * @param powerLedDiodePin Pin number for the power LED diode.
 * @param wifiLedDiodePin Pin number for the WiFi LED diode.
 * @param pumpLedDiodePin Pin number for the pump LED diode.
 * @param vegetableLedDiodePin Pin number for the vegetable LED diode.
 * @param flowerLedDiodePin Pin number for the flower LED diode.
 * @param bluePWMPin PWM pin for controlling blue color on the LED strip.
 * @param redPWMPin PWM pin for controlling red color on the LED strip.
 * @param greenPWMPin PWM pin for controlling green color on the LED strip.
 */
LEDController::LEDController(
    ShiftRegister* shiftRegister, 
    uint8_t powerLedDiodePin, 
    uint8_t wifiLedDiodePin, 
    uint8_t pumpLedDiodePin, 
    uint8_t vegetableLedDiodePin, 
    uint8_t flowerLedDiodePin, 
    uint8_t bluePWMPin, 
    uint8_t redPWMPin, 
    uint8_t greenPWMPin): 
        shiftRegister(shiftRegister), 
        powerLedDiodePin(powerLedDiodePin),
        wifiLedDiodePin(wifiLedDiodePin), 
        pumpLedDiodePin(pumpLedDiodePin), 
        vegetableLedDiodePin(vegetableLedDiodePin), 
        flowerLedDiodePin(flowerLedDiodePin), 
        bluePWMPin(bluePWMPin), 
        redPWMPin(redPWMPin), 
        greenPWMPin(greenPWMPin), 
        wifiManager(nullptr), 
        ledBlinkState(false), 
        lastBlinkMillis(0), 
        wifiBlinkCounter(0), 
        blinkInterval(200){ 
            ledcSetup(0, 5000, 8);
            ledcSetup(1, 5000, 8);
            ledcSetup(2, 5000, 8);
            ledcAttachPin(bluePWMPin, 0);
            ledcAttachPin(redPWMPin, 1);
            ledcAttachPin(greenPWMPin, 2);
}

/**
 * Sets the WiFiManager to enable LED control based on WiFi connection status.
 * 
 * @param manager Reference to a WiFiManager object.
 */
void LEDController::setWiFiManager(WiFiManager& manager) {
    wifiManager = &manager;
}

/**
 * Updates the state of the WiFi LED based on connection status.
 * 
 * @param isConnected Indicates whether the WiFi is connected.
 */
void LEDController::updateWiFiLedDiodeStatus(bool isConnected) {
    if (isConnected) {
        // WiFi is connected
        shiftRegister->setPinState(wifiLedDiodePin, HIGH);
        shiftRegister->write();
        ledBlinkState = false;
        wifiBlinkCounter = 0;
    } else {
        // WiFi is disconnected or connecting
        if (wifiBlinkCounter < 3) {
            blinkWiFiLedDiode();
            wifiBlinkCounter++;
        } else {
            shiftRegister->setPinState(wifiLedDiodePin, LOW);
            shiftRegister->write();
            ledBlinkState = false;
        }
    }
}

/**
 * Blinks the WiFi LED a specified number of times.
 * 
 * @param count Number of blink cycles.
 */
void LEDController::blinkWiFiLedDiode(int count) {
    static unsigned long lastBlinkTime = 0;
    static bool blinkState = false;
    static int blinkCounter = 0;

    if (millis() - lastBlinkTime >= blinkInterval) {
        blinkState = !blinkState;
        shiftRegister->setPinState(wifiLedDiodePin, blinkState);
        shiftRegister->write();
        lastBlinkTime = millis();

        if (blinkState) {
            blinkCounter++;
            if (blinkCounter >= count * 2) {
                blinkCounter = 0;
            }
        }
    }
}

/**
 * Sets the state of an individual LED diode.
 * 
 * @param ledDiode The LED diode to modify.
 * @param ledDiodeState The desired state (true for on, false for off).
 */
void LEDController::setLedDiodeState(DiodeType ledDiode, bool ledDiodeState) {
    uint8_t pin = getLedDiodePin(ledDiode);
    shiftRegister->setPinState(pin, ledDiodeState);
    shiftRegister->write();
}

/**
 * Toggles the state of an individual LED diode.
 * 
 * @param ledDiode The LED diode to toggle.
 */
void LEDController::toggleLedDiodeState(DiodeType ledDiode) {
    uint8_t pin = getLedDiodePin(ledDiode);
    bool currentLedDiodeState = shiftRegister->getPinState(pin);
    shiftRegister->setPinState(pin, !currentLedDiodeState);
    shiftRegister->write();
}

/**
 * Sets the LED strip mode (color or off) based on the input mode.
 * 
 * @param ledStripMode Mode to set for the LED strip.
 */
void LEDController::setLedStripMode(uint8_t ledStripMode) {
    switch (ledStripMode) {
        case 0:
            ledcWrite(0, 255);
            ledcWrite(1, 0);
            ledcWrite(2, 0);
            break;
        case 1:
            ledcWrite(0, 0);
            ledcWrite(1, 255);
            ledcWrite(2, 0);
            break;
        case 2:
            ledcWrite(0, 0);
            ledcWrite(1, 0);
            ledcWrite(2, 0);
            break;
    }
}

/**
 * Retrieves the pin number associated with a given LED diode type.
 * 
 * @param diode The diode type for which to get the pin number.
 * @return The pin number associated with the specified diode type.
 */
uint8_t LEDController::getLedDiodePin(DiodeType ledDiodePin) const {
    switch(ledDiodePin) {
        case DiodeType::Power: return powerLedDiodePin;
        case DiodeType::WiFi: return wifiLedDiodePin;
        case DiodeType::Pump: return pumpLedDiodePin;
        case DiodeType::Vegetable: return vegetableLedDiodePin;
        case DiodeType::Flower: return flowerLedDiodePin;
        default: 
            DebugLogger::error("Unknown diode type.");
            return 255; // Invalid pin number
    }
}