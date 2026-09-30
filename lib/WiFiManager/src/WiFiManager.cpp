#include "WiFiManager.hpp"
#include "DebugLogger.hpp"

// Static member initialization.
bool WiFiManager::connected = false;
bool WiFiManager::connecting = false;

/**
 * Constructs a WiFiManager to manage WiFi connections.
 *
 * @param ssid WiFi network SSID.
 * @param password WiFi network password.
 */
WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid),
      password(password),
      startTime(0),
      lastAttemptTime(0),
      connectAttempts(0) {
}

/**
 * Starts a WiFi connection attempt without blocking the main loop.
 */
void WiFiManager::connect() {
    if (isConnected() || isConnecting()) {
        return;
    }

    // WiFi may have been disabled by disconnect().
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    connecting = true;
    connected = false;
    startTime = millis();
    lastAttemptTime = startTime;

    // Records each connection attempt for external observers.
    ++connectAttempts;

    DebugLogger::info("Attempting to connect to WiFi...");
}

/**
 * Handles connection success, timeout, disconnection and reconnection.
 */
void WiFiManager::handleConnectionResult() {
    const unsigned long currentTime = millis();
    const wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {
        if (!connected) {
            connected = true;
            connecting = false;

            DebugLogger::info("Successfully connected to WiFi.");
            DebugLogger::info("SSID: " + String(WiFi.SSID()));
            DebugLogger::info("IP Address: " + WiFi.localIP().toString());
        }

        return;
    }

    // Handle the loss of an established connection.
    if (connected) {
        connected = false;
        connecting = false;
        lastAttemptTime = currentTime;

        DebugLogger::info("WiFi connection lost.");
    }

    // Terminate an attempt that has exceeded the timeout.
    if (connecting && currentTime - startTime >= attemptInterval) {
        DebugLogger::info("WiFi connection attempt timed out.");

        WiFi.disconnect();
        connecting = false;
        connected = false;
        lastAttemptTime = currentTime;

        return;
    }

    // Wait before starting the next attempt.
    if (!connecting && currentTime - lastAttemptTime >= attemptInterval) {
        DebugLogger::info("Retrying WiFi connection...");
        connect();
    }
}

/**
 * Disconnects from WiFi.
 */
void WiFiManager::disconnect() {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);

    connected = false;
    connecting = false;

    DebugLogger::info("Disconnected from WiFi.");
}

/**
 * Returns true while a connection attempt is active.
 */
bool WiFiManager::isConnecting() {
    return connecting;
}

/**
 * Returns true when WiFi is connected.
 */
bool WiFiManager::isConnected() {
    connected = WiFi.status() == WL_CONNECTED;

    if (connected) {
        connecting = false;
    }

    return connected;
}

/**
 * Returns the number of connection attempts started.
 */
uint32_t WiFiManager::getConnectAttempts() const {
    return connectAttempts;
}