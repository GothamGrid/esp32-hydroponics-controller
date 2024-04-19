// DebugLogger.cpp
#include "DebugLogger.hpp"

bool DebugLogger::isDebugEnabled = false;

/**
 * @brief Logs an informational message.
 * @param message The message to be logged.
 */
void DebugLogger::info(const String& message) {
    if (isDebugEnabled) {
        Serial.println("[INFO] " + message);
    }
}

/**
 * @brief Logs an error message.
 * @param message The message to be logged.
 */
void DebugLogger::error(const String& message) {
    if (isDebugEnabled) {
        Serial.println("[ERROR] " + message);
    }
}

/**
 * @brief Enables or disables debug logging.
 * @param enable True to enable debug logging, false to disable.
 */
void DebugLogger::setDebug(bool enable) {
    isDebugEnabled = enable;
    if (enable) {
        Serial.begin(115200);
    } else {
        Serial.end();
    }
}