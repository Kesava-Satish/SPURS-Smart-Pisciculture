# SPURS: Smart Pisciculture User Recommendation System

## Project Overview
SPURS is an IoT-enabled aquaculture management system designed to optimize yield and reduce operational risks in fish farming. The system monitors **6 critical water parameters** in real-time and utilizes a **breed-specific algorithm** to provide actionable recommendations for **12 different fish species**.

This project was published in **IEEE Xplore**.

## Key Features
* **Multi-Parameter Sensing:** Real-time monitoring of pH, Temperature, Dissolved Oxygen (DO), Turbidity, and Water Level.
* **Species-Specific Logic:** A custom algorithm compares sensor data against a database of optimal and critical thresholds for specific fish breeds (e.g., Rohu, Catla, Tilapia).
* **User Recommendation Engine:** Instead of just displaying raw data, the system outputs direct advice (e.g., "Add Lime," "Turn on Aerator") to the user via a mobile interface.
* **Operational Impact:** Proven to reduce annual operational costs for manual testing by **70%**.

## Technical Architecture
* **Hardware:** ESP32 Microcontroller, DS18B20 Temperature Sensor, Analog pH Sensor, DO Probe, Ultrasonic Sensor.
* **Communication:** Wi-Fi (MQTT/HTTP) for cloud data logging.
* **Algorithm:** Threshold-based decision tree derived from aquacultural biological standards.

## Methodology
The system operates on a feedback loop:
1.  **Sensing:** Sensors acquire water quality metrics.
2.  **Processing:** The microcontroller filters noise and computes averages.
3.  **Analysis:** The `recommendation_algo` compares current metrics against the `critical_values_datasheet`.
4.  **Action:** Alerts are generated if parameters deviate from the "Optimal" range into the "Critical" range.

## Repository Structure
* `/src`: Complete set of codes used for the project.
* `/firmware`: C++ code for the ESP32 sensor node.
* `/data`: Datasets defining optimal and critical parameters for supported fish species.
* `/docs`: Conference paper and project documentation.

## Publication
**Implementation of Internet of Things enabled Smart Pisciculture User Recommendation System**
* **Status:** Published
* **Platform:** IEEE Xplore

## Author
**Kesava Satish Boppana**
