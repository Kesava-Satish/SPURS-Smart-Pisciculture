#include <SoftwareSerial.h>

#define SSID "vedi pradesam"
#define PASS "12345678912"

SoftwareSerial espSerial(0, 1); // RX, TX

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  sendCommand("AT+RST\r\n", 2000);
  sendCommand("AT+CWMODE=1\r\n", 1000);
  sendCommand("AT+CWJAP=\"" SSID "\",\"" PASS "\"\r\n", 5000);
  sendCommand("AT+CIFSR\r\n", 1000);
}

void loop() {
  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
  sendCommand(cmd, 5000);
  cmd = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
  sendCommand("AT+CIPSEND=" + String(cmd.length()) + "\r\n", 1000);
  sendCommand(cmd, 5000);
  delay(5000);
}

void sendCommand(String cmd, int timeout) {
  Serial.print("Sending command: ");
  Serial.print(cmd);

  espSerial.print(cmd);
  long start = millis();
  while ((millis() - start) < timeout) {
    while (espSerial.available()) {
      char c = espSerial.read();
      Serial.print(c);
    }
  }
  Serial.println();
}
