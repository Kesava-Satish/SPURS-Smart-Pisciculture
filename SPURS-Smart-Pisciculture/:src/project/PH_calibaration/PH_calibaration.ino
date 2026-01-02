int pH_Value;
float Voltage;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pH_Value,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pH_Value = analogRead(55);
 Voltage= pH_Value *(5.0/1023.0);
  Serial.println(Voltage);
  delay(1000);
}
