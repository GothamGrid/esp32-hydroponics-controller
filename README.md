# ESP32 Hydroponics Controller

The ESP32 Hydroponics Controller controls lighting and a water pump in a hydroponic setup using an ESP32 microcontroller. The current firmware supports physical button controls, LED indicators, LED strip modes, pump control, and WiFi connection management.

Remote control through a web interface is planned but is not yet implemented.

## Features

- **Lighting Control**: Select vegetable and flower LED strip modes using physical buttons, and manage individual status indicators.
- **Water Pump Management**: Control pump operation using a physical button, with pump state managed through `PumpController` and `AppState`.
- **WiFi Connectivity**: Manage connection attempts, timeouts, connection loss, and reconnection without blocking the main loop.
- **WiFi Status Indication**: Display WiFi activity using complete, non-blocking LED blink sequences.
- **Modular Design**: Dedicated components handle buttons, LEDs, pump operation, WiFi, shift-register outputs, application state, and logging.
- **Debug Logging**: View diagnostic messages through the serial monitor, with optional ESP-Prog hardware debugging.

Automatic lighting and pump schedules, Bluetooth control, sensor measurements, analytics, alerts, and web/mobile control are not documented as implemented features.

## Getting Started

### Prerequisites

To build and run the current configuration, you will need:

- ESP32 development board compatible with the `esp32dev` PlatformIO configuration
- Physical buttons, status LEDs, and a shift register
- LED strip with suitable driver circuitry
- Water pump with suitable switching/driver circuitry
- External 12 V DC power supply
- USB cable for uploads and serial monitoring
- PlatformIO CLI or PlatformIO with VS Code
- Optional ESP-Prog for JTAG uploads and hardware debugging

The current board configuration and GPIO assignments target a classic ESP32. The planned ESP32-C3-MINI-1 migration requires different board settings and a review of GPIO assignments and JTAG connections. Do not reuse this configuration unchanged for that module.

The system power budget is **12 V DC, 2 A (24 W total)**. Keep connected loads and power-conversion circuitry within that budget. Do not power the pump or LED strip directly from GPIO or shift-register outputs; use suitable driver circuitry.

Keep mains-powered components external to the controller enclosure. Use splash-resistant enclosures and protect connections from water exposure.

### Installation

1. Clone the [repository](https://github.com/GothamGrid/esp32-hydroponics-controller.git):

   ```bash
   git clone https://github.com/GothamGrid/esp32-hydroponics-controller.git
   cd esp32-hydroponics-controller
   ```

2. Open the project directory in VS Code with PlatformIO, or use the PlatformIO CLI from that directory.

3. Edit the existing `Config.hpp` to match your hardware wiring. Keep it in the location used by the project's includes. The following example matches the supplied pin assignments:

   ```cpp
   #ifndef CONFIG_HPP
   #define CONFIG_HPP

   // Use only one WiFi credential source; see the guidance below.
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASS "your_wifi_password"

   // Physical button GPIOs.
   #define POWER_BUTTON_PIN 21
   #define PUMP_BUTTON_PIN 22
   #define VEGETABLE_BUTTON_PIN 17
   #define FLOWER_BUTTON_PIN 16

   // Shift-register interface GPIOs.
   #define SHIFT_REGISTER_DATA_PIN 18
   #define SHIFT_REGISTER_CLOCK_PIN 23
   #define SHIFT_REGISTER_LATCH_PIN 19

   // Shift-register output indices, not ESP32 GPIO numbers.
   #define POWER_DIODE_PIN 0
   #define WIFI_DIODE_PIN 1
   #define PUMP_MOTOR_PIN 2
   #define PUMP_DIODE_PIN 3
   #define VEGETABLE_DIODE_PIN 4
   #define FLOWER_DIODE_PIN 5

   // LED strip PWM GPIOs.
   #define BLUE_PWM_PIN 25
   #define RED_PWM_PIN 26
   #define GREEN_PWM_PIN 27

   #define WIFI_BLINK_COUNT 3
   #define LOOP_DELAY 10
   #define VEGETABLE_ON 0
   #define FLOWER_ON 1
   #define STRIP_OFF 2

   // Current classic ESP32 external JTAG GPIOs.
   #define JTAG_TMS_PIN 14
   #define JTAG_TDI_PIN 12
   #define JTAG_TCK_PIN 13
   #define JTAG_TDO_PIN 15

   #endif // CONFIG_HPP
   ```

   Verify all assignments against your wiring. Change hardware pin assignments in `Config.hpp`, not throughout `main.cpp`. The JTAG macros document wiring; they do not configure PlatformIO's debug transport.

   **WiFi credentials:** The supplied `Config.hpp` defines `WIFI_SSID` and `WIFI_PASS`, while `platformio.ini` also defines them through environment-variable build flags. Choose one source:

   - For header-based credentials, keep the definitions in `Config.hpp` and remove the corresponding WiFi build flags.
   - For environment-based credentials, remove the definitions from `Config.hpp`, set `WIFI_SSID` and `WIFI_PASS` in PlatformIO's environment, and verify that the build flags produce valid C++ string literals.

   Do not commit actual credentials. If the header contains credentials, add its actual repository-relative path to `.gitignore`. Ignoring a file does not untrack it if it is already committed. Replace any credentials previously exposed in Git history.

4. Connect the board to your computer using USB. List serial devices:

   ```bash
   pio device list
   ```

   Update `[ports]` in `platformio.ini` to match your computer:

   ```ini
   [ports]
   board_serial = /dev/cu.usbserial-1410
   probe_serial = /dev/cu.usbserial-14101
   ```

   These example paths come from the existing macOS setup. Device names may change with the computer, operating system, USB port, or hub. `board_serial` identifies the board's USB serial interface; `probe_serial` identifies the ESP-Prog UART monitoring interface. JTAG uses the probe's USB debug interface, not this UART path.

5. Use the dependencies actually required by the project. Do not add unrelated libraries from generic examples. The project's `WiFiManager.hpp` and `WiFiManager.cpp` do not, by name alone, imply a dependency on the external `WiFiManager` library.

6. Select the appropriate PlatformIO environment:

   - `esp32dev`: Board USB uploads and serial monitoring.
   - `esp32-jtag`: ESP-Prog JTAG uploads and hardware debugging, with UART monitoring when separately wired.

   In VS Code, use the environment selector or environment-specific Project Tasks. In CLI commands, use `-e`. This is **environment selection**, not switching Git branches. The option applies only to its individual command. Commands without an explicit environment use the configured default, currently `esp32dev`.

7. Upload and monitor the firmware.

   **Board USB upload:**

   ```bash
   pio run -e esp32dev -t upload
   ```

   **Board USB: clean, build, upload, and monitor:**

   ```bash
   pio run -e esp32dev -t clean && pio run -e esp32dev && pio run -e esp32dev -t upload && pio device monitor -e esp32dev -b 115200 -f direct
   ```

   **ESP-Prog upload:**

   ```bash
   pio run -e esp32-jtag -t upload
   ```

   **ESP-Prog: clean, build, upload, and monitor:**

   ```bash
   pio run -e esp32-jtag -t clean && pio run -e esp32-jtag && pio run -e esp32-jtag -t upload && pio device monitor -e esp32-jtag -b 115200 -f direct
   ```

   Repeat `-e` for every command in a chain. `&&` proceeds only when the preceding command succeeds. Cleaning is optional and forces a fresh rebuild. Upload also checks/builds the firmware as needed. The monitor uses 115200 baud and the `direct` filter; this does not guarantee noise-free output.

   **Switching to ESP-Prog:** Stop monitoring/debugging and power down before changing wiring. Connect the correctly oriented JTAG ribbon, follow the board/probe power requirements, and avoid conflicting supplies. For this classic ESP32 setup, keep GPIO 12–15 free of external loads while the JTAG ribbon is connected.

   ESP-Prog serial monitoring requires separate UART wiring to the board. The JTAG ribbon alone does not carry serial logs.

   Start hardware debugging separately:

   ```bash
   pio debug -e esp32-jtag
   ```

   Alternatively, start the corresponding PlatformIO debug configuration in VS Code.

   **Switching back to board USB:** Stop monitoring/debugging and power down. Disconnect the JTAG ribbon if those pins are needed by peripherals, restore normal wiring, connect board USB, and select `esp32dev`. Selecting an environment changes tool configuration, not physical wiring.

   **Debug source paths:** `[debug_paths]` centralizes the source mappings used by `debug_extra_cmds`. Build paths come from library debug information; local paths must point to matching sources on your computer. These mappings do not download sources. Review them when your installation, framework, or toolchain changes.

8. After uploading, use the physical buttons to control power, pump operation, and lighting modes. Inspect diagnostic logs through the serial monitor.

   Web and mobile control are not yet available. The planned web implementation will use `SystemController` as a common command layer for buttons and web requests, with `WebServerManager` handling web access. Both input paths should keep hardware outputs and `AppState` consistent.

   For oscilloscope measurements, connect ground clips only to circuit 0 V/GND. Use an appropriately rated differential probe for non-ground-referenced measurements.

### Contributing

Contributions, suggestions, and bug reports are welcome through the [GitHub repository](https://github.com/GothamGrid/esp32-hydroponics-controller.git).

Follow the existing code style and conventions, and provide clear, concise commit messages. Discuss major changes in an issue before implementation. Keep documentation aligned with implemented behavior and clearly label planned functionality.

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/GothamGrid/esp32-hydroponics-controller/blob/master/LICENSE.txt) file for more information.
