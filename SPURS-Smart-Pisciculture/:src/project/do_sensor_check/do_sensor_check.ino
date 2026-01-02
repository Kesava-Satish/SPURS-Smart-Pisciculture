const int sensorPin = 57;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
int sensorValue = analogRead(sensorPin);

  // Convert the sensor value to dissolved oxygen level (adjust this conversion based on your sensor's specifications)
  float dissolvedOxygen = map(sensorValue, 0, 1023, 0, 100); // Example conversion from 0-1023 to 0-100%

  // Print the dissolved oxygen level to the serial monitor
  Serial.print("Dissolved Oxygen: ");
  Serial.print(dissolvedOxygen);
  Serial.println("%");
  delay(2000);
}
