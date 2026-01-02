// Define the input pin for the turbidity sensor
const int turbiditySensorPin =56 ;

void setup() {
  // Set up serial communication at 9600 baud
  Serial.begin(9600);
}

void loop() {
  // Read the raw analog value from the turbidity sensor
  float turbidityValue = analogRead(turbiditySensorPin);
  
  // Convert the raw value to turbidity in NTUs
  float turbidity = map(turbidityValue, 0,750,100,0);
  
  // Print the turbidity value to the serial monitor
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  //Serial.println(turbidityValue);
  Serial.println(" NTU");
  
  // Delay for 1 second
  delay(1000);
}
