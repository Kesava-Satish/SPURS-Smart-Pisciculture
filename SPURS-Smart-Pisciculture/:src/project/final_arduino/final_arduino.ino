const int UPDATE_INTERVAL = 5000;
// Variables for the temperature and humidity values
const int pHSensorPin = 55;
//const int temperatureSensorPin =32;
int buffer_arr[10],temp;
float ph,temperature;

// Define the voltage offset and slope values for the pH sensor calibration
const float pHOffset = 21.34 -0.6;
const float pHSlope = -5.70;

//Define ultrasonic sensor pins
const int trigPin = 53;
const int echoPin = 52;
float duration, distance,dis1;

//turbidity pin
const int turbiditySensorPin = 56;

//tds sensor
int tdssensorPin = 54;
int sensorValue = 0;
float tdsValue = 0;
float Voltage = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  perhydro();
  ultrasonic();
  turbidity();
  totaldissolve();
//  totaldo();
  delay(UPDATE_INTERVAL);
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
}

void turbidity(){
  float turbidityValue = analogRead(turbiditySensorPin);
  
  // Convert the raw value to turbidity in NTUs
  float turbidity = map(turbidityValue, 0,750,100,0);
  
  // Print the turbidity value to the serial monitor
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  //Serial.println(turbidityValue);
  Serial.println(" NTU");
 }

 void totaldissolve(){
  sensorValue = analogRead(tdssensorPin);
    Voltage = sensorValue*5/1024.0; //Convert analog reading to Voltage
    tdsValue=(133.42/Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
    Serial.print("TDS Value = "); 
    Serial.print(tdsValue);
    Serial.println(" ppm");
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
  float pHVoltage = (float)pHValue* 5/ 1024.0/6;
  
  // Convert the voltage to pH using the calibration parameters
  float pH = pHSlope * pHVoltage + pHOffset;
  
  // Print the pH and temperature values to the serial monitor
  Serial.print("pH: ");
  Serial.print(pH);
 }
