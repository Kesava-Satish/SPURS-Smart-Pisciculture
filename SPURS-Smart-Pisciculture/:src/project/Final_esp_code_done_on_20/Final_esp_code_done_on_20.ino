#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// Constants for the Wi-Fi network
const char* ssid = "vedi pradesam";           // your network SSID (name)
const char* password = "1234567891";   // your network password

// Constants for the ThingSpeak channel
const unsigned long CHANNEL_ID = 2080763 ;         // your ThingSpeak channel ID
const char* WRITE_API_KEY = "5V80L26UXATTAPTX";  // your ThingSpeak write API key

const int UPDATE_INTERVAL = 30000;
// Wi-Fi client
WiFiClient client;
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Initialize ThingSpeak library
  ThingSpeak.begin(client);
  // Configure ESP32 settings and setup ThingSpeak connection
}

void loop() {
  if (Serial.available()>0) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println(receivedData);
    String sensorDataString = receivedData.substring(0);
    String sensorValues[5];
    int index=0;
    while (sensorDataString.length() > 0) {
        int commaIndex = sensorDataString.indexOf(',');
        if (commaIndex != -1) {
          sensorValues[index] = sensorDataString.substring(0, commaIndex);
          sensorDataString = sensorDataString.substring(commaIndex + 1);
        } else {
          sensorValues[index] = sensorDataString;
          sensorDataString = "";
        }
        index++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        ThingSpeak.setField(1, sensorValues[0]);
        ThingSpeak.setField(2, sensorValues[1]);
        ThingSpeak.setField(3, sensorValues[2]);
        ThingSpeak.setField(4, sensorValues[3]);
        ThingSpeak.setField(5, sensorValues[4]);
        //ThingSpeak.setField(6, sensorValues[5].toInt());
        Serial.println(sensorValues[0]);
        Serial.println(sensorValues[1]);
        Serial.println(sensorValues[2]);
        Serial.println(sensorValues[3]);
        Serial.println(sensorValues[4]);
        int response = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
        
        if (response == 200) {
          Serial.println("Data sent to ThingSpeak successfully");
        } else {
          Serial.println("Failed to send data to ThingSpeak");
        }
        
       delay(UPDATE_INTERVAL);
      }}}
