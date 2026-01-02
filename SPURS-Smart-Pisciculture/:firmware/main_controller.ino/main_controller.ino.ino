/*
 * SPURS - IoT Node Firmware
 * Device: ESP32 / NodeMCU
 * Sensors: DS18B20 (Temp), Analog pH, Analog Turbidity, DO Probe
 */

#include <WiFi.h>

// Sensor Pins
#define TEMP_PIN 4
#define PH_PIN 34
#define DO_PIN 35

float temperature, ph_val, do_val;

void setup() {
  Serial.begin(115200);
  // Initialize Sensors
  initTempSensor();
  initPhSensor();
}

void loop() {
  // 1. Acquire Data
  temperature = readTemp();
  ph_val = readPH();
  do_val = readDO();

  // 2. Serial Output for Debugging
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" | pH: "); Serial.print(ph_val);
  Serial.print(" | DO: "); Serial.println(do_val);

  // 3. Send to Cloud (Mock function)
  sendToCloud(temperature, ph_val, do_val);

  delay(2000);
}

// Mock readout functions
float readTemp() { return 28.5; } 
float readPH() { return 7.2; }
float readDO() { return 5.5; }
void sendToCloud(float t, float p, float d) {
  // MQTT or HTTP Post logic goes here
}
void initTempSensor() {}
void initPhSensor() {}