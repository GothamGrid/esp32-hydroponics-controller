#ifndef WiFiManager_h
#define WiFiManager_h

#include <Arduino.h>
#include <WiFi.h>

/**
 * Manages WiFi connectivity, providing methods to connect, disconnect, and check connection status.
 */
class WiFiManager {
public:
    /**
     * Constructor.
     * Initializes a new WiFiManager instance for managing WiFi connections.
     *
     * @param ssid WiFi network SSID.
     * @param password WiFi network password.
     */
    WiFiManager(const char* ssid, const char* password);

    /**
     * Initiates a non-blocking connection to the specified WiFi network.
     */
    void connect();

    /**
     * Monitors and manages the WiFi connection status, attempting reconnections if necessary.
     * Call this method regularly, such as in the main application loop.
     */
    void handleConnectionResult();

    /**
     * Disconnects from the currently connected WiFi network.
     */
    void disconnect();

    /**
     * Checks if the device is currently trying to connect to a WiFi network.
     *
     * @return True if attempting to connect, false otherwise.
     */
    bool isConnecting();

    /**
     * Checks if the device is currently connected to a WiFi network.
     *
     * @return True if connected, false otherwise.
     */
    bool isConnected();

private:
    const char* ssid; // SSID of the WiFi network
    const char* password; // Password of the WiFi network
    static bool connecting; // Flag indicating if a connection attempt is ongoing
    static bool connected; // Flag indicating if the device is currently connected
    unsigned long startTime; // Timestamp of the connection attempt start
    unsigned long lastAttemptTime; // Timestamp of the last connection attempt
    const unsigned long attemptInterval = 5000; // Interval between connection attempts (ms)
};

#endif /* WiFiManager_h */
