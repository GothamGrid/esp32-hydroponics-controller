#ifndef DiodeTypes_hpp
#define DiodeTypes_hpp

/**
 * @file DiodeTypes.hpp
 * @brief Defines the DiodeType enumeration used across the LEDController system.
 */

/**
 * @enum DiodeType
 * @brief Enumeration for identifying specific diodes within the LED control system.
 * 
 * This enumeration simplifies method calls and enhances readability across
 * the system by providing a clear, unified way to refer to different diodes.
 */
enum class DiodeType {
    Power,      // Represents the power diode.
    WiFi,       // Represents the WiFi diode.
    Pump,       // Represents the pump diode.
    Vegetable,  // Represents the vegetable diode.
    Flower      // Represents the flower diode.
};

#endif /* DiodeTypes_hpp */