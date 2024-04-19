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
#include "DebugLogger.hpp"

AppState appState;

// Object initialization with configuration parameters.
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

// Button identifiers for readability.
enum Button { Power, Pump, Vegetable, Flower };


// Forward declaration for a function handling LED and LED strip logic.
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
    ledController.setWiFiManager(wifiManager);
    ledController.tuneMultipleLedAttributes(
        DiodeType::Power, false, 
        DiodeType::WiFi, false, 
        DiodeType::Pump, false,
        DiodeType::Vegetable, false,
        DiodeType::Flower, false
    );
    ledController.setLedStripMode(STRIP_OFF);
    
    // Initialize application states with default values
    appState.setPowerState(false);
    appState.setWiFiLedDiodeState(false);
    appState.setPumpLedDiodeState(false);
    appState.setVegetableLedDiodeState(false);
    appState.setFlowerLedDiodeState(false);
    appState.setLedStripState(false);

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
            ledController.tuneMultipleLedAttributes(
                DiodeType::Power, true, 
                DiodeType::WiFi, true
            );
        }
    } else {
        appState.setPowerState(false);
        DebugLogger::info("System powered down.");
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
    }
}

/**
 * @brief Handles pump button click events.
 * 
 * Toggles the state of the pump LED when the pump button is clicked.
 */
void handlePumpButtonClick() {
    if (appState.isPowerOn()) {
        ledController.toggleLedDiodeState(DiodeType::Pump);
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
 * @brief Updates WiFi LED state based on current WiFi connection status.
 */
void updateWiFiLedDiodeState() {
    if (wifiManager.isConnecting()) {
        ledController.blinkWiFiLedDiode(WIFI_BLINK_COUNT);
    } else if (wifiManager.isConnected()) {
        ledController.setLedDiodeState(DiodeType::WiFi, true);
    } else {
        ledController.setLedDiodeState(DiodeType::Pump, false);
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

    if (allButtons[Power].isClicked()) handlePowerButtonClick();
    if (allButtons[Pump].isClicked()) handlePumpButtonClick();
    if (allButtons[Vegetable].isClicked()) handleVegetableButtonClick();
    if (allButtons[Flower].isClicked()) handleFlowerButtonClick();
    if (appState.isPowerOn()) {
        wifiManager.handleConnectionResult();
        updateWiFiLedDiodeState();
    }
    delay(10);
}

