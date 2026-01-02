//#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h> 
#include <ThingSpeak.h>
#include"DHT.h"
#define DHTPIN 56 // DHT data pin
#define DHTTYPE DHT11
#define RX 0 // TX of esp8266 in connected with Arduino pin 2
#define TX 1 // RX of esp8266 in connected with Arduino pin 3 
DHT dht(DHTPIN, DHTTYPE);
String WIFI_SSID = "vedi pradesam";// WIFI NAME
String WIFI_PASS = "12345678912"; // WIFI PASSWORD
String API = "5V80L26UXATTAPTX";// Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false;   
SoftwareSerial esp(RX,TX); 
void setup() {
  Serial.begin(9600);
  esp.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ WIFI_SSID +"\",\""+ WIFI_PASS +"\"",20,"OK");
}
void loop() {
 float temperature = dht.readTemperature();
 float humidity =dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("Humidity: ");
  Serial.print(humidity);
 String getData="GET /update?api_key="+ API+"&field1="+temperature+"&field2="+humidity;
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp.println(command);//at+cipsend
    if(esp.find(readReplay))//ok
    {
      found = true;
      break;
    }
    countTimeCommand++;
  } 
  if(found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  } 
  found = false;
 }








/*
// Replace with your network credentials
const char* ssid = "Satish's iPhone";
const char* password = "satishkumar07";

// Replace with your ThingSpeak API key
const char* apiKey = "5V80L26UXATTAPTX";

// Initialize the ESP8266 client object
WiFiClient client;

void setup() {
   dht.begin();
  Serial.begin(9600);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak client
  ThingSpeak.begin(client);
}

void loop() {
  // Read sensor data and update ThingSpeak channel

  float temperature = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C ");// Replace with your sensor data
  ThingSpeak.writeField(2080763, 1, temperature, apiKey);
  delay(10000); // Wait 10 seconds before sending next update
}*/
