#include "LEDController.hpp"
#include "DebugLogger.hpp"

/**
 * Constructs an LEDController.
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
    uint8_t greenPWMPin
) :
    shiftRegister(shiftRegister),
    powerLedDiodePin(powerLedDiodePin),
    wifiLedDiodePin(wifiLedDiodePin),
    pumpLedDiodePin(pumpLedDiodePin),
    vegetableLedDiodePin(vegetableLedDiodePin),
    flowerLedDiodePin(flowerLedDiodePin),
    bluePWMPin(bluePWMPin),
    redPWMPin(redPWMPin),
    greenPWMPin(greenPWMPin),

    // ★ WiFi blink-sequencer initialization.
    wifiBlinkActive(false),
    wifiBlinkState(false),
    wifiBlinkTransitionsRemaining(0),
    lastWiFiBlinkMillis(0) {

    ledcSetup(0, 5000, 8);
    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);

    ledcAttachPin(bluePWMPin, 0);
    ledcAttachPin(redPWMPin, 1);
    ledcAttachPin(greenPWMPin, 2);
}

/**
 * Starts a complete non-blocking WiFi LED blink sequence.
 *
 * One blink consists of one ON phase and one OFF phase.
 */
void LEDController::startWiFiBlink(uint8_t count) {
    if (count == 0) {
        cancelWiFiBlink();
        return;
    }

    wifiBlinkActive = true;
    wifiBlinkState = true;

    // ★ The first ON state is applied immediately. Every remaining state
    // ★ change is performed later by update().
    wifiBlinkTransitionsRemaining =
        static_cast<uint16_t>(count) * 2U - 1U;

    lastWiFiBlinkMillis = millis();

    shiftRegister->setPinState(wifiLedDiodePin, HIGH);
    shiftRegister->write();
}

/**
 * Advances the active WiFi LED blink sequence.
 */
void LEDController::update() {
    if (!wifiBlinkActive) {
        return;
    }

    const unsigned long currentTime = millis();

    if (currentTime - lastWiFiBlinkMillis < wifiBlinkInterval) {
        return;
    }

    lastWiFiBlinkMillis = currentTime;
    wifiBlinkState = !wifiBlinkState;

    shiftRegister->setPinState(
        wifiLedDiodePin,
        wifiBlinkState ? HIGH : LOW
    );
    shiftRegister->write();

    if (wifiBlinkTransitionsRemaining > 0) {
        --wifiBlinkTransitionsRemaining;
    }

    if (wifiBlinkTransitionsRemaining == 0) {
        // A complete sequence always finishes with the LED off.
        wifiBlinkActive = false;
        wifiBlinkState = false;

        shiftRegister->setPinState(wifiLedDiodePin, LOW);
        shiftRegister->write();
    }
}

/**
 * Returns whether the WiFi LED blink sequence is active.
 */
bool LEDController::isWiFiBlinking() const {
    return wifiBlinkActive;
}

/**
 * Cancels the WiFi LED blink sequence and turns the diode off.
 */
void LEDController::cancelWiFiBlink() {
    wifiBlinkActive = false;
    wifiBlinkState = false;
    wifiBlinkTransitionsRemaining = 0;

    shiftRegister->setPinState(wifiLedDiodePin, LOW);
    shiftRegister->write();
}

/**
 * Sets the state of an individual LED diode.
 */
void LEDController::setLedDiodeState(
    DiodeType ledDiode,
    bool ledDiodeState
) {
    const uint8_t pin = getLedDiodePin(ledDiode);

    shiftRegister->setPinState(pin, ledDiodeState);
    shiftRegister->write();
}

/**
 * Toggles an individual LED diode.
 */
void LEDController::toggleLedDiodeState(DiodeType ledDiode) {
    const uint8_t pin = getLedDiodePin(ledDiode);
    const bool currentState = shiftRegister->getPinState(pin);

    shiftRegister->setPinState(pin, !currentState);
    shiftRegister->write();
}

/**
 * Sets the LED strip mode.
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

        default:
            DebugLogger::error("Unknown LED strip mode.");
            break;
    }
}

/**
 * Returns the configured pin for a diode type.
 */
uint8_t LEDController::getLedDiodePin(
    DiodeType ledDiodePin
) const {
    switch (ledDiodePin) {
        case DiodeType::Power:
            return powerLedDiodePin;

        case DiodeType::WiFi:
            return wifiLedDiodePin;

        case DiodeType::Pump:
            return pumpLedDiodePin;

        case DiodeType::Vegetable:
            return vegetableLedDiodePin;

        case DiodeType::Flower:
            return flowerLedDiodePin;

        default:
            DebugLogger::error("Unknown diode type.");
            return 255;
    }
}