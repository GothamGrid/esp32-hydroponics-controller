# ESP32 Hydroponics Controller

The ESP32 Hydroponics Controller is an advanced system designed to automate and optimize hydroponic plant growth using an ESP32 microcontroller. With WiFi and Bluetooth connectivity, this project enables remote control and monitoring of lighting and water pump schedules, ensuring optimal growth conditions and efficient water usage.

## Features

- **Intelligent Lighting Control**: Automate and fine-tune lighting schedules to simulate natural day/night cycles, promoting healthy plant growth and development. Remotely adjust light intensity and duration to cater to the specific needs of different plant species.

- **Precise Water Pump Management**: Efficiently manage water circulation schedules to ensure plants receive the optimal amount of water and nutrients at the right intervals. Remotely control and monitor pump operation to maintain ideal moisture levels and prevent over or under-watering.

- **Wireless Connectivity**: Leverage the power of WiFi and Bluetooth to control and monitor your hydroponic system remotely. Access real-time data, adjust settings, and receive alerts from anywhere using a user-friendly web interface or mobile app.

- **Scalable and Customizable**: Designed to be modular and adaptable to various hydroponic setups, from small-scale indoor systems to large commercial operations. Easily expand and customize the controller to accommodate additional sensors, actuators, and features as your needs evolve.

- **Data Logging and Analytics**: Record and analyze crucial growth parameters such as temperature, humidity, pH levels, and nutrient concentrations. Gain valuable insights into your plants' health and make data-driven decisions to optimize your hydroponic system's performance.

## Getting Started

### Prerequisites

To build and run the ESP32 Hydroponics Controller, you will need:

- ESP32 Development Board
- Relay modules for controlling lighting and water pumps
- Water pump suitable for your hydroponic setup
- LED grow lights
- Arduino IDE or PlatformIO with VSCode

### Installation

1. Clone the repository to your local machine: [REPOSITORY](https://github.com/GothamGrid/esp32-hydroponics-controller.git)

2. Open the project with your preferred IDE (Arduino IDE or PlatformIO with VSCode).

3. Before compiling and uploading the code to your ESP32, you need to configure the GPIO pins according to your specific hardware setup. These configurations are crucial for the proper functioning of your hydroponics controller, including managing lighting, water pumps, and sensor data.

Create or edit the Config.h file in the src directory of your project, and define your GPIO pin assignments and other necessary configurations there. Here is an example configuration with pin assignments:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// GPIO pin assignments
#define POWER_BUTTON_PIN 32
#define PUMP_BUTTON_PIN 33
#define VEGETABLE_BUTTON_PIN 25
#define FLOWER_BUTTON_PIN 26

#define SHIFT_REGISTER_DATA_PIN 14
#define SHIFT_REGISTER_CLOCK_PIN 27
#define SHIFT_REGISTER_LATCH_PIN 12

#define POWER_DIODE_PIN 16
#define WIFI_DIODE_PIN 17
#define PUMP_DIODE_PIN 18
#define VEGETABLE_DIODE_PIN 19
#define FLOWER_DIODE_PIN 21

#define BLUE_PWM_PIN 22
#define RED_PWM_PIN 23
#define GREEN_PWM_PIN 5

// Add any other configuration variables here

#endif // CONFIG_H

```

Make sure to replace the pin numbers with those that correspond to your actual hardware setup. Also, ensure you replace your_wifi_ssid and your_wifi_password with your actual WiFi credentials.

To keep your WiFi credentials and pin configurations secure, make sure the Config.h file is listed in your .gitignore file to prevent it from being committed to your repository:

```
# Ignore local configuration
src/Config.h
```

By following these steps, you ensure that your ESP32 Hydroponics Controller is correctly configured for your specific setup, enhancing its functionality and reliability.

4. Connect the ESP32 board to your computer using a USB cable.

5. Open the `platformio.ini` file and install the necessary libraries by adding them under the lib_deps section. For example:

```
lib_deps =
    WiFiManager
    PubSubClient
    Adafruit_Sensor
    DHT
```

6. Check the configuration of the GPIO pins in the code according to your hardware setup. If necessary, modify the pin assignments in the `src/main.cpp` file to match your connections.

7. Upload the sketch to your ESP32 board by clicking the "Upload" button in your IDE or running the following command in the terminal:

```
pio run -t upload
```

However, if you want to first clean the project, then build it, and finally upload and monitor the INPUT & OUTPUT stream with the baud rate option set to 115200 and the text transformation filter that forwards all data unchanged, run the following command:

```
pio run -t clean && pio run && pio run -t upload && pio device monitor -b 115200 -f direct
```

This will ensure a clean and unobstructed output in the terminal.

8. Once the upload is complete, the ESP32 Hydroponics Controller will start running, and you can interact with it using the provided web interface or mobile app.

### Contributing

We welcome contributions from the community! If you have any suggestions, bug reports, or would like to add new features, please feel free to submit a pull request or open an issue on the GitHub repository.
When contributing, please adhere to the existing code style and conventions, and provide clear and concise commit messages. If you are proposing major changes, it's recommended to discuss them first by opening an issue to ensure alignment with the project's goals.

## License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/GothamGrid/esp32-hydroponics-controller/blob/master/LICENSE.txt) file for more information.