#include <Arduino.h>

#define VREF 5000
#define ADC_RES 1024

const int doSensorPin = A3; // Analog pin connected to the DO sensor

float convertToDO(uint32_t sensorValue);

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint32_t raw = analogRead(doSensorPin);
  float doConcentration = convertToDO(raw);
  
  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("\tVoltage (mV): ");
  Serial.print(raw * VREF / ADC_RES);
  Serial.print("\tDO Concentration (mg/L): ");
  Serial.println(doConcentration);

  delay(2000);
}

float convertToDO(uint32_t sensorValue) {
  // Add your conversion formula here
  // Replace this with the actual conversion formula provided by the sensor manufacturer
  float doConcentration = map(sensorValue, 0, 1023, 0, 100);
  
  return doConcentration;
}
