// DebugLogger.h
#ifndef DebugLogger_h
#define DebugLogger_h

#include <Arduino.h>

/**
 * @brief Provides static methods for logging debug information.
 */
class DebugLogger {
public:
    /**
     * @brief Logs an informational message.
     * @param message The message to be logged.
     */
    static void info(const String& message);

    /**
     * @brief Logs an error message.
     * @param message The message to be logged.
     */
    static void error(const String& message);

    /**
     * @brief Enables or disables debug logging.
     * @param enable True to enable debug logging, false to disable.
     */
    static void setDebug(bool enable);

private:
    static bool isDebugEnabled; // Flag to indicate if debug logging is enabled.
};

#endif