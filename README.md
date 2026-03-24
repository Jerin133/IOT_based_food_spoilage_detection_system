# Food Spoilage Detection System (IoT)

This project uses an **ESP32** and an **MQ-series gas sensor** to monitor the freshness of food in real-time. It provides local alerts and remote notifications via the **Blynk IoT** platform.

---

## Table of Contents
* [Features](#-features)
* [Hardware Components](#-hardware-components)
* [Circuit Connections](#-circuit-connections)
* [Software Setup](#-software-setup)
* [How It Works](#-how-it-works)

---

## Features
- **Continuous Monitoring:** Real-time detection of methane and organic gases.
- **Visual & Audio Alerts:** Green LED for "Fresh", Red LED + Buzzer for "Spoiled".
- **IoT Integration:** Remote data viewing on the Blynk App.
- **Smart Notifications:** Automated **Gmail** and **Push Notifications** when spoilage is detected.
- **Warm-up Logic:** Built-in 30-second delay to stabilize the sensor on startup.

---

## Hardware Components
| Component | Specification |
| :--- | :--- |
| **Microcontroller** | ESP32 Dev Module |
| **Gas Sensor** | MQ-4 / MQ-135 |
| **LEDs** | 5mm Red and Green |
| **Buzzer** | 5V Active Piezo Buzzer |
| **Resistors** | 220 Ohm (for LEDs) |

---

## Circuit Connections
> **Note:** Ensure all GND pins are common for stable analog readings.

* **MQ Sensor VCC** -> ESP32 5V (or VIN)
* **MQ Sensor GND** -> ESP32 GND
* **MQ Sensor AO** -> ESP32 **GPIO 34**
* **Red LED** -> ESP32 **GPIO 2**
* **Green LED** -> ESP32 **GPIO 15**
* **Buzzer** -> ESP32 **GPIO 4**

## Software Setup

### 1. Libraries Required
Install these via the Arduino Library Manager:
- `WiFi.h` (Built-in for ESP32)
- `Blynk` by Volodymyr Shymanskyy

### 2. Configuration
Update the following lines in the code with your specific credentials:
```cpp
char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_WiFi_Name";
char pass[] = "Your_WiFi_Password";
```

## Threshold Calibration
The **ESP32** features a 12-bit Analog-to-Digital Converter (ADC), which provides a reading range from **0 to 4095**. Because the MQ gas sensor requires a heating element, initial readings may spike before stabilizing.

| Status | ADC Value | Hardware Response |
| :--- | :--- | :--- |
| **Warm-up** | First 30s | System ignores readings to prevent false alarms. |
| **Fresh/Safe** | < 2000 | Green LED remains **ON**. |
| **Spoiled/Danger**| > 2000 | Red LED & Buzzer **ON**. Blynk event triggered. |

---

## Notification Logic
The system utilizes the `Blynk.logEvent()` function to bridge the gap between hardware and the user's smartphone/inbox.

### 1. The Trigger
When the gas concentration crosses the **2000** threshold, the ESP32 sends a signal to the Blynk Cloud:
`Blynk.logEvent("food_spoiled", "Warning: Food Spoilage Detected!");`

### 2. The Delivery
The Blynk Cloud is configured with a **Critical Event** handler:
* **Event Code:** `food_spoiled`
* **Recipient:** Device Owner (Gmail)
* **Frequency:** Limited to 1 notification per minute to prevent spamming the user's inbox.

### 3. Visual Dashboard
The real-time methane value is continuously streamed to **Virtual Pin V12**, which can be viewed on a Gauge or Chart widget in the Blynk App.

---
