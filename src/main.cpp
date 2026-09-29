/**
 * @file main.cpp
 * @brief Main application file for an ESP32-based smart gardening system.
 * 
 * This file contains the setup and main loop for an ESP32 project designed for smart gardening. 
 * It initializes the system, manages button events, LED states, and WiFi connectivity.
 */

#include "Config.hpp"
#include "AppState.hpp"
#include "WiFiManager.hpp"
#include "ButtonManager.hpp"
#include "LEDController.hpp"
#include "ShiftRegister.hpp"
#include "PumpController.hpp"
#include "DebugLogger.hpp"

// ★ Used to detect each connection attempt exactly once.
uint32_t lastWiFiAttemptCount = 0;

AppState appState;

// ★ Object initialization with configuration parameters.
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

// ★ Create an instance of the pump controller
PumpController pumpController(shiftRegister);

// ★ Button identifiers for readability.
enum Button { Power, Pump, Vegetable, Flower };


// ★ Forward declaration for a function handling LED and LED strip logic.
void handleMultipleLedInteractions(
    bool& currentLedDiodeState, 
    bool& otherLedDiodeState, 
    DiodeType currentLedDiode, 
    DiodeType otherLedDiode, 
    uint8_t ledStripMode
);

/**
 * @brief Initializes the system components.
 * 
 * Configures system peripherals, initializes the WiFi manager, and sets initial LED states.
 */
void setup() {
    DebugLogger::setDebug(true);
    for (auto& button : allButtons) {
        button.setup();
    }

    // ★ REMOVED: ledController.setWiFiManager(wifiManager);
    // ★ LEDController no longer owns or observes WiFiManager.

    ledController.tuneMultipleLedAttributes(
        DiodeType::Power, false, 
        DiodeType::WiFi, false, 
        DiodeType::Pump, false,
        DiodeType::Vegetable, false,
        DiodeType::Flower, false
    );
    ledController.setLedStripMode(STRIP_OFF);
    
    // ★ Initialize application states with default values
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
 * @brief Toggles the system's power state on power button press.
 * 
 * Manages the system power state, initiates or disconnects WiFi connection, and updates LED states.
 */
void handlePowerButtonClick() {
    if (!appState.isPowerOn()) {
        if (!wifiManager.isConnecting() && !wifiManager.isConnected()) {
            appState.setPowerState(true);
            DebugLogger::info("System powered up.");

            wifiManager.connect();
            // ★ CHANGED: power LED is on, but the WiFi LED is controlled by
            // ★ the non-blocking blink sequence and connection state.
            ledController.tuneMultipleLedAttributes(
                DiodeType::Power, true,
                DiodeType::WiFi, false
            );
            appState.setWiFiLedDiodeState(false);

        }
    } else {
        appState.setPowerState(false);
        DebugLogger::info("System powered down.");

        // ★ Prevent an in-progress blink from continuing after power-off.
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

         // ★ Turn off the motor pump via PumpController
        pumpController.setMotorPumpState(false);
        // ★ Store the state in AppState
        appState.setMotorPumpState(false);

        // ★ Log the motor pump state
        pumpController.getMotorPumpState();

    }
}

/**
 * @brief Handles pump button click events.
 * 
 * Toggles the state of the pump LED and motor pump when the pump button is clicked.
 */
void handlePumpButtonClick() {
    if (appState.isPowerOn()) {
        // ★ Toggle the pump LED state
        ledController.toggleLedDiodeState(DiodeType::Pump);

        // ★ Directly toggle the motor pump state
        bool currentMotorPumpState = pumpController.getMotorPumpState();
        bool newMotorPumpState = !currentMotorPumpState;

        // ★ Update the motor pump state via PumpController
        pumpController.setMotorPumpState(newMotorPumpState);

        // ★ Store the state in AppState
        appState.setMotorPumpState(newMotorPumpState);

        // ★ Log the motor pump state
        DebugLogger::info("Pump button clicked. New motor pump state: " + String(newMotorPumpState ? "ON" : "OFF"));
    }
}

/**
 * @brief Handles vegetable button click events.
 * 
 * Manages the LED strip state and color based on the vegetable button's state.
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
 * @brief Handles flower button click events.
 * 
 * Manages the LED strip state and color based on the flower button's state.
 */
void handleFlowerButtonClick() {
    if(appState.isPowerOn()) {
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
 * @brief Toggles LED states and updates LED strip mode based on user interaction.
 * 
 * Switches the state of two specified LEDs, ensuring only one is active at any time. Sets the LED strip to the appropriate mode.
 * 
 * @param currentLedDiodeState Reference to the current LED's state.
 * @param otherLedDiodeState Reference to the other LED's state.
 * @param currentLedDiode Current LED being manipulated.
 * @param otherLedDiode Other LED potentially affected by the interaction.
 * @param ledStripMode Mode for the LED strip (0 for vegetable, 1 for flower, 2 for off).
 */
void handleMultipleLedInteractions(
    bool& currentLedDiodeState,
    bool& otherLedDiodeState,
    DiodeType currentLedDiode,
    DiodeType otherLedDiode,
    uint8_t ledStripMode) {
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
 * @brief Main loop of the application.
 * 
 * Continuously checks and updates button states, processes button click events, and manages WiFi LED status.
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

        /*
         * Start the blink sequence exactly once whenever WiFiManager
         * starts a new connection attempt.
         */
        const uint32_t attempts =
            wifiManager.getConnectAttempts();

        if (attempts != lastWiFiAttemptCount) {
            lastWiFiAttemptCount = attempts;

            ledController.startWiFiBlink(
                WIFI_BLINK_COUNT
            );
        }

        /*
         * Do not overwrite the diode while the blink sequencer owns it.
         * Once blinking finishes, show the actual connection state.
         */
        if (!ledController.isWiFiBlinking()) {
            const bool wifiConnected =
                wifiManager.isConnected();

            ledController.setLedDiodeState(
                DiodeType::WiFi,
                wifiConnected
            );

            appState.setWiFiLedDiodeState(
                wifiConnected
            );
        }
    }

    // ★ Advances the non-blocking LED sequence.
    ledController.update();

    /*
     * ★ OPTIONAL TEST HOOK:
     *
     * Enter lowercase `b` in the serial monitor to test three blinks
     * independently of the router and connection speed.
     */
    if (Serial.available() > 0 &&
        Serial.read() == 'b') {

        ledController.startWiFiBlink(
            WIFI_BLINK_COUNT
        );
    }

    delay(10);
}