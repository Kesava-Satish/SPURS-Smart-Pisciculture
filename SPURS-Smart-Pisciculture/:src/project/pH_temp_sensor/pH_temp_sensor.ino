// Define the input pins for the pH sensor and temperature sensor
const int pHSensorPin = 55;
//const int temperatureSensorPin = 32;
int buffer_arr[10],temp;

// Define the voltage offset and slope values for the pH sensor calibration
const float pHOffset = 21.34 + 1.94;
const float pHSlope = -5.70;

void setup() {
  // Set up serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
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
  float pHVoltage = (float)pHValue* 5/ 1024.0/6;
  
  // Convert the voltage to pH using the calibration parameters
  float pH = pHSlope * pHVoltage + pHOffset;
  
  // Read the raw analog value from the temperature sensor
  //int temperatureValue = analogRead(temperatureSensorPin);
  
  // Convert the raw value to temperature in Celsius
  //float temperature = (float)temperatureValue / 4096.0 * 5.0;
  //temperature = (temperature - 0.5) * 100.0 - 375.0;
  
  // Print the pH and temperature values to the serial monitor
  Serial.print("pH: ");
  Serial.print(pH);
  //Serial.print(", Temperature: ");
 // Serial.print(temperature);
 // Serial.println(" °C");
  
  // Delay for 2 second
  delay(2000);
}
