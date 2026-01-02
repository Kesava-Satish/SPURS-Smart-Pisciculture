void setup() {
  Serial.begin(9600);
  // Configure ESP32 settings and setup ThingSpeak connection
}

void loop() {
  if (Serial.available()>0) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println(receivedData);
}}
