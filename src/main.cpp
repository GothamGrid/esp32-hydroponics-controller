/**
 * @file main.cpp
 * @brief Main application file for an ESP32-based smart gardening system.
 *
 * Initializes system components and manages button events, LED states,
 * pump control, and WiFi connectivity.
 */

#include "Config.hpp"
#include "AppState.hpp"
#include "WiFiManager.hpp"
#include "ButtonManager.hpp"
#include "LEDController.hpp"
#include "ShiftRegister.hpp"
#include "PumpController.hpp"
#include "DebugLogger.hpp"

// Tracks connection attempts to trigger each blink sequence once.
uint32_t lastWiFiAttemptCount = 0;

AppState appState;

WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);

ButtonManager allButtons[] = {
    ButtonManager(POWER_BUTTON_PIN),
    ButtonManager(PUMP_BUTTON_PIN),
    ButtonManager(VEGETABLE_BUTTON_PIN),
    ButtonManager(FLOWER_BUTTON_PIN)
};

ShiftRegister shiftRegister(
    SHIFT_REGISTER_DATA_PIN,
    SHIFT_REGISTER_CLOCK_PIN,
    SHIFT_REGISTER_LATCH_PIN
);

LEDController ledController(
    &shiftRegister,
    POWER_DIODE_PIN,
    WIFI_DIODE_PIN,
    PUMP_DIODE_PIN,
    VEGETABLE_DIODE_PIN,
    FLOWER_DIODE_PIN,
    BLUE_PWM_PIN,
    RED_PWM_PIN,
    GREEN_PWM_PIN
);

PumpController pumpController(shiftRegister);

enum Button { Power, Pump, Vegetable, Flower };

void handleMultipleLedInteractions(
    bool& currentLedDiodeState,
    bool& otherLedDiodeState,
    DiodeType currentLedDiode,
    DiodeType otherLedDiode,
    uint8_t ledStripMode
);

/**
 * @brief Configures buttons and initializes LED outputs and application states.
 */
void setup() {
    DebugLogger::setDebug(true);

    for (auto& button : allButtons) {
        button.setup();
    }

    ledController.tuneMultipleLedAttributes(
        DiodeType::Power, false,
        DiodeType::WiFi, false,
        DiodeType::Pump, false,
        DiodeType::Vegetable, false,
        DiodeType::Flower, false
    );
    ledController.setLedStripMode(STRIP_OFF);

    appState.setPowerState(false);
    appState.setWiFiLedDiodeState(false);
    appState.setPumpLedDiodeState(false);
    appState.setVegetableLedDiodeState(false);
    appState.setFlowerLedDiodeState(false);
    appState.setLedStripState(false);
    appState.setMotorPumpState(false);

    DebugLogger::info("System initialized and ready.");
}

/**
 * @brief Handles power button clicks.
 *
 * Manages system power, WiFi connectivity, LED outputs, and pump shutdown.
 */
void handlePowerButtonClick() {
    if (!appState.isPowerOn()) {
        if (!wifiManager.isConnecting() && !wifiManager.isConnected()) {
            appState.setPowerState(true);
            DebugLogger::info("System powered up.");

            wifiManager.connect();

            // The WiFi LED is controlled by the blink sequence and connection state.
            ledController.tuneMultipleLedAttributes(
                DiodeType::Power, true,
                DiodeType::WiFi, false
            );
            appState.setWiFiLedDiodeState(false);
        }
    } else {
        appState.setPowerState(false);
        DebugLogger::info("System powered down.");

        // Prevent an in-progress blink from continuing after power-off.
        ledController.cancelWiFiBlink();
        wifiManager.disconnect();

        ledController.tuneMultipleLedAttributes(
            DiodeType::Power, false,
            DiodeType::WiFi, false,
            DiodeType::Pump, false,
            DiodeType::Vegetable, false,
            DiodeType::Flower, false
        );

        ledController.setLedStripMode(STRIP_OFF);
        appState.setWiFiLedDiodeState(false);
        appState.setPumpLedDiodeState(false);
        appState.setVegetableLedDiodeState(false);
        appState.setFlowerLedDiodeState(false);
        appState.setLedStripState(false);

        pumpController.setMotorPumpState(false);
        appState.setMotorPumpState(false);

        pumpController.getMotorPumpState();
    }
}

/**
 * @brief Toggles the pump LED and motor pump when the system is powered on.
 */
void handlePumpButtonClick() {
    if (appState.isPowerOn()) {
        ledController.toggleLedDiodeState(DiodeType::Pump);

        bool currentMotorPumpState = pumpController.getMotorPumpState();
        bool newMotorPumpState = !currentMotorPumpState;

        pumpController.setMotorPumpState(newMotorPumpState);
        appState.setMotorPumpState(newMotorPumpState);

        DebugLogger::info(
            "Pump button clicked. New motor pump state: " +
            String(newMotorPumpState ? "ON" : "OFF")
        );
    }
}

/**
 * @brief Handles vegetable button clicks and updates lighting states.
 */
void handleVegetableButtonClick() {
    if (appState.isPowerOn()) {
        bool currentVegetableLedDiodeState = appState.getStateForLedDiode(DiodeType::Vegetable);
        bool currentFlowerLedDiodeState = appState.getStateForLedDiode(DiodeType::Flower);

        handleMultipleLedInteractions(
            currentVegetableLedDiodeState,
            currentFlowerLedDiodeState,
            DiodeType::Vegetable,
            DiodeType::Flower,
            0
        );

        appState.setVegetableLedDiodeState(currentVegetableLedDiodeState);
        appState.setFlowerLedDiodeState(currentFlowerLedDiodeState);
    } else if (appState.isVegetableLedDiodeOn()) {
        appState.setVegetableLedDiodeState(false);
    }

    DebugLogger::info("Vegetable Button State: " + String(appState.isVegetableLedDiodeOn()));
}

/**
 * @brief Handles flower button clicks and updates lighting states.
 */
void handleFlowerButtonClick() {
    if (appState.isPowerOn()) {
        bool currentFlowerLedDiodeState = appState.getStateForLedDiode(DiodeType::Flower);
        bool currentVegetableLedDiodeState = appState.getStateForLedDiode(DiodeType::Vegetable);

        handleMultipleLedInteractions(
            currentFlowerLedDiodeState,
            currentVegetableLedDiodeState,
            DiodeType::Flower,
            DiodeType::Vegetable,
            1
        );

        appState.setFlowerLedDiodeState(currentFlowerLedDiodeState);
        appState.setVegetableLedDiodeState(currentVegetableLedDiodeState);
    } else if (appState.isFlowerLedDiodeOn()) {
        appState.setFlowerLedDiodeState(false);
    }

    DebugLogger::info("Flower Button State: " + String(appState.isFlowerLedDiodeOn()));
}

/**
 * @brief Toggles mutually exclusive lighting modes.
 *
 * Activates the selected diode and strip mode, or switches the lighting off
 * when the selected diode is already active.
 *
 * @param currentLedDiodeState Reference to the selected diode's state.
 * @param otherLedDiodeState Reference to the other diode's state.
 * @param currentLedDiode Selected diode.
 * @param otherLedDiode Other diode to deactivate.
 * @param ledStripMode Strip mode to activate (0 for vegetable, 1 for flower).
 */
void handleMultipleLedInteractions(
    bool& currentLedDiodeState,
    bool& otherLedDiodeState,
    DiodeType currentLedDiode,
    DiodeType otherLedDiode,
    uint8_t ledStripMode
) {
    if (!currentLedDiodeState) {
        ledController.setLedDiodeState(currentLedDiode, true);
        ledController.setLedDiodeState(otherLedDiode, false);
        currentLedDiodeState = true;
        otherLedDiodeState = false;
        ledController.setLedStripMode(ledStripMode);
        appState.setLedStripState(true);
    } else {
        ledController.setLedDiodeState(currentLedDiode, false);
        currentLedDiodeState = false;

        if (otherLedDiodeState) {
            ledController.setLedDiodeState(otherLedDiode, false);
            otherLedDiodeState = false;
        }

        ledController.setLedStripMode(STRIP_OFF);
        appState.setLedStripState(false);
    }
}

/**
 * @brief Processes button events, WiFi connectivity, and LED updates.
 */
void loop() {
    for (auto& button : allButtons) {
        button.update();
    }

    if (allButtons[Power].isClicked()) {
        handlePowerButtonClick();
    }

    if (allButtons[Pump].isClicked()) {
        handlePumpButtonClick();
    }

    if (allButtons[Vegetable].isClicked()) {
        handleVegetableButtonClick();
    }

    if (allButtons[Flower].isClicked()) {
        handleFlowerButtonClick();
    }

    if (appState.isPowerOn()) {
        wifiManager.handleConnectionResult();

        // Start a blink sequence once for each new connection attempt.
        const uint32_t attempts = wifiManager.getConnectAttempts();

        if (attempts != lastWiFiAttemptCount) {
            lastWiFiAttemptCount = attempts;

            ledController.startWiFiBlink(WIFI_BLINK_COUNT);
        }

        // Do not overwrite the diode while the blink sequencer owns it.
        // Once blinking finishes, show the actual connection state.
        if (!ledController.isWiFiBlinking()) {
            const bool wifiConnected = wifiManager.isConnected();

            ledController.setLedDiodeState(DiodeType::WiFi, wifiConnected);
            appState.setWiFiLedDiodeState(wifiConnected);
        }
    }

    // Advance the non-blocking LED sequence.
    ledController.update();

    // Serial test hook: enter 'b' to start a WiFi LED blink sequence.
    if (Serial.available() > 0 && Serial.read() == 'b') {
        ledController.startWiFiBlink(WIFI_BLINK_COUNT);
    }

    delay(10);
}