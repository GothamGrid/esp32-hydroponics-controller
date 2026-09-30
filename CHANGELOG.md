# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.2] - 2026-09-29
### Added
- **LEDController**: Added non-blocking WiFi blink sequencing, status reporting, and cancellation.
- **WiFiManager**: Added connection-attempt tracking.
- **PlatformIO**: Added centralized serial ports and debug source-path settings.
- **README**: Added USB and ESP-Prog workflows with explicit environment selection.

### Changed
- **WiFiManager**: Improved connection, timeout, disconnection, and reconnection handling.
- **LEDController**: Separated WiFi connection management from LED control.
- **Code Cleanup**: Standardized comments and documentation in the main application, LED controller, and WiFi manager.
- **PlatformIO**: Updated configuration to use centralized port and path variables.
- **README**: Updated `Config.hpp`, credential names, GPIO assignments, and hardware guidance.
- **README**: Distinguished implemented functionality from planned web access and MCU migration.

### Removed
- **Code Cleanup**: Removed temporary markers, obsolete comments, and unused code.
- **LEDController**: Removed obsolete WiFi-management dependencies and declarations.
- **README**: Removed unsupported feature claims and speculative dependency examples.

### Fixed
- **LEDController**: Corrected incomplete WiFi LED blink sequences.
- **README**: Corrected outdated configuration names, pins, and commands without environment selection.

## [1.0.1] - 2024-05-18
### Added
- **PumpController**: Added to manage the motor pump via the shift register.
- **AppState**: Added motor pump state management methods.

### Changed
- **handlePumpButtonClick**: Updated to directly toggle motor pump state.
- **PumpController**: Enhanced to include state logging and direct control via `setPinState`.

## [1.0.0] - 2024-04-18
### Added
- **LEDController**: Comprehensive control over individual and groups of LEDs.
- **WiFiManager**: Simplified WiFi connectivity and management.
- **ShiftRegister**: Integration for expanded I/O capabilities.
- **DebugLogger**: Streamlined debugging and logging.
- **ButtonManager**: Improved button press handling and debouncing.
- **AppState**: Centralized state management class added for better management of system states such as power, LED diodes statuses, and LED strip states.

### Initial
- Initial release laying the groundwork for a robust, user-friendly control system for hydroponic environments.

### Fixed
- Refined **WiFiManager** connection logic to handle edge cases and improve reliability.
- Enhanced **LEDController** for more efficient LED state management.
- Improved **DebugLogger** format for better readability and consistency.
- Updated **ButtonManager** debouncing mechanism for more accurate button press detection.
- Created **AppState** for centralized state management, enhancing the control flow and state consistency across the application.

## [Template for New Entries]

### Added
- For new features.
### Changed
- For changes in existing functionality.
### Deprecated
- For soon-to-be removed features.
### Removed
- For now removed features.
### Fixed
- For any bug fixes.
### Security
- In case of vulnerabilities.
