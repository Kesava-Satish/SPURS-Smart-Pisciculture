const int trigPin = 53;
const int echoPin = 52;

float duration, distance,dis1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
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
  delay(2000);
}
