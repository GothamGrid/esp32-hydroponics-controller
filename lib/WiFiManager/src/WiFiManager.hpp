#ifndef WiFiManager_h
#define WiFiManager_h

#include <Arduino.h>
#include <WiFi.h>

/**
 * Manages WiFi connectivity, providing methods to connect,
 * disconnect and check connection status.
 */
class WiFiManager {
public:
    /**
     * Initializes a WiFiManager instance.
     *
     * @param ssid WiFi network SSID.
     * @param password WiFi network password.
     */
    WiFiManager(const char* ssid, const char* password);

    /**
     * Starts a non-blocking WiFi connection attempt.
     */
    void connect();

    /**
     * Processes connection success, connection timeout and reconnection.
     *
     * This function is non-blocking and should be called regularly.
     */
    void handleConnectionResult();

    /**
     * Disconnects from WiFi and disables the WiFi radio.
     */
    void disconnect();

    /**
     * Checks whether a connection attempt is active.
     */
    bool isConnecting();

    /**
     * Checks whether WiFi is currently connected.
     */
    bool isConnected();

    // ★ Allows main.cpp to detect each new connection attempt once.
    uint32_t getConnectAttempts() const;

private:
    const char* ssid;
    const char* password;

    static bool connecting;
    static bool connected;

    unsigned long startTime;
    unsigned long lastAttemptTime;

    // ★ Monotonically increasing connection-attempt counter.
    uint32_t connectAttempts;

    // ★ Also used as the connection-attempt timeout.
    const unsigned long attemptInterval = 5000;
};

#endif /* WiFiManager_h */