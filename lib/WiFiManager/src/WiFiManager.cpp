#include "WiFiManager.hpp"
#include "DebugLogger.hpp"

// Static member initialization
bool WiFiManager::connected = false;
bool WiFiManager::connecting = false;

/**
 * Constructs a WiFiManager to manage WiFi connections.
 *
 * @param ssid WiFi network SSID.
 * @param password WiFi network password.
 */
WiFiManager::WiFiManager(const char* ssid, const char* password)
: ssid(ssid), password(password), startTime(0), lastAttemptTime(0) {}

/**
 * Initiates connection to a WiFi network without blocking.
 */
void WiFiManager::connect() {
    if (!isConnected() && !isConnecting()) {
        WiFi.begin(ssid, password);
        connecting = true;
        startTime = millis();
        DebugLogger::info("Attempting to connect to WiFi...");
    }
}

/**
 * Manages WiFi connection status, attempting reconnections as needed.
 * Should be called regularly to ensure continuous connectivity.
 */
void WiFiManager::handleConnectionResult() {
    unsigned long currentTime = millis();
    if (connecting) {
        if (WiFi.status() == WL_CONNECTED) {
            if (!connected) {
                DebugLogger::info("Successfully connected to WiFi.");
                DebugLogger::info("SSID: " + String(WiFi.SSID()));
                DebugLogger::info("IP Address: " + WiFi.localIP().toString());
                connected = true;
            }
            connecting = false;
        } else if (currentTime - lastAttemptTime > attemptInterval && !connecting) {
            DebugLogger::info("Attempting to reconnect to WiFi...");
            connect();
            lastAttemptTime = currentTime;
        }
    } else if (!connecting && connected && WiFi.status() != WL_CONNECTED) {
        DebugLogger::info("WiFi disconnected. Attempting to reconnect...");
        connect();
        lastAttemptTime = currentTime;
    } else if (!connecting && !connected) {
        DebugLogger::info("Attempting to connect to WiFi...");
        connect();
        lastAttemptTime = currentTime;
    }
    if (WiFi.status() != WL_CONNECTED) {
        DebugLogger::info(".");
        delay(250);
    }
}

/**
 * Disconnects from the WiFi network, ensuring disconnection is confirmed.
 */
void WiFiManager::disconnect() {
    if (WiFi.disconnect()) {
        unsigned long startMillis = millis();
        while (WiFi.status() != WL_DISCONNECTED && (millis() - startMillis <= 5000)) {}
        if (WiFi.status() == WL_DISCONNECTED) {
            DebugLogger::info("Disconnected from WiFi.");
        } else {
            DebugLogger::info("Disconnection timeout.");
        }
        WiFi.mode(WIFI_OFF);
        connected = false;
        connecting = false;
    }
}

/**
 * Returns true if a connection attempt is ongoing.
 *
 * @return True if connecting, false otherwise.
 */
bool WiFiManager::isConnecting() {
    return connecting;
}

/**
 * Returns true if connected to a WiFi network.
 *
 * @return True if connected, false otherwise.
 */
bool WiFiManager::isConnected() {
    connected = WiFi.status() == WL_CONNECTED;
    return connected;
}
