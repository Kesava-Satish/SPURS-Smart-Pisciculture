#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// Constants for the Wi-Fi network
const char* ssid = "vedi pradesam";           // your network SSID (name)
const char* password = "1234567891";   // your network password

// Constants for the ThingSpeak channel
const unsigned long CHANNEL_ID = 2080763 ;         // your ThingSpeak channel ID
const char* WRITE_API_KEY = "5V80L26UXATTAPTX";  // your ThingSpeak write API key

// Constants for the update interval
const int UPDATE_INTERVAL = 30000; // update interval in milliseconds

// Variables for the temperature and humidity values
//float temperature = 0.0;
//float humidity = 0.0;
const int pHSensorPin = 33;
const int temperatureSensorPin =32;
int buffer_arr[10],temp;
float ph,temperature;

// Define the voltage offset and slope values for the pH sensor calibration
const float pHOffset = 21.34 - 0.6;
const float pHSlope = -5.70;

//Define ultrasonic sensor pins
const int trigPin = 5;
const int echoPin = 18;
float duration, distance,dis1;
const int turbiditySensorPin = 56;
int sensorPin = 12;
int sensorValue = 0;
float tdsValue = 0;
float Voltage = 0;

//Define dissolved oxygen sensor pins
const int sensorPin =30;

// Wi-Fi client
WiFiClient client;

// Setup function
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  
  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Initialize ThingSpeak library
  ThingSpeak.begin(client);
}

// Loop function
void loop() {
  perhydro();
  ultrasonic();
  turbidity();
  totaldissolve();
  totaldo();
  
  int response = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  
  // Check for successful update
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }
  
  // Delay for update interval
  delay(UPDATE_INTERVAL);
}

void perhydro(){
  int pHValue = analogRead(pHSensorPin);
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(pHSensorPin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 pHValue=0;
 for(int i=2;i<8;i++){
  pHValue+=buffer_arr[i];}
  // Read the raw analog value from the pH sensor
  
  
  // Convert the raw value to voltage
  float pHVoltage = (float)pHValue* 0.84/ 1024.0/6;
  
  // Convert the voltage to pH using the calibration parameters
  float pH = pHSlope * pHVoltage + pHOffset;
  
  // Read the raw analog value from the temperature sensor
  int temperatureValue = analogRead(temperatureSensorPin);
  
  // Convert the raw value to temperature in Celsius
  float temperature = (float)temperatureValue / 4096 * 5.0;
  temperature = (temperature - 0.5) * 100.0 - 375.0;
  
  // Print the pH and temperature values to the serial monitor
  Serial.print("pH: ");
  Serial.print(pH);
  Serial.print(", Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, pH);
 }

 void ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  dis1 = distance * .393701;
  Serial.print("Distance: ");
  Serial.print(dis1);
  Serial.println("inches");
  ThingSpeak.setField(3,dis1);
  }
  
 void turbidity(){
  float turbidityValue = analogRead(turbiditySensorPin);
  
  // Convert the raw value to turbidity in NTUs
  float turbidity = map(turbidityValue, 0,3069,100,0);
  
  // Print the turbidity value to the serial monitor
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  //Serial.println(turbidityValue);
  Serial.println(" NTU");
  ThingSpeak.setField(4,turbidity);
 }

 void totaldissolve(){
  sensorValue = analogRead(sensorPin);
    Voltage = sensorValue*5/4096.0; //Convert analog reading to Voltage
    tdsValue=(133.42/Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
    Serial.print("TDS Value = "); 
    Serial.print(tdsValue);
    Serial.println(" ppm");
    ThingSpeak.setField(5,tdsValue);
 }

 void totaldo(){
  // Read the sensor value
  int sensorValue = analogRead(sensorPin);

  // Convert the sensor value to dissolved oxygen level (adjust this conversion based on your sensor's specifications)
  float dissolvedOxygen = map(sensorValue, 0, 1023, 0, 100); // Example conversion from 0-1023 to 0-100%

  // Print the dissolved oxygen level to the serial monitor
  Serial.print("Dissolved Oxygen: ");
  Serial.print(dissolvedOxygen);
  Serial.println("%");
  ThingSpeak.setField(6,dissolvedOxygen);
}
