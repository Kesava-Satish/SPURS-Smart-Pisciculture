#include <OneWire.h>
#include <DallasTemperature.h>


#define DO_PIN A1
 
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution
 
//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 0
 
#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function
 
//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (1455) //mv
#define CAL1_T (25)   //℃
//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point
#define CAL2_V (1300) //mv
#define CAL2_T (15)   //℃
 
const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};
 
uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
uint16_t DO;

int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
#if TWO_POINT_CALIBRATION == 00
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#endif
}

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

//temp sensor pins
#define ONE_WIRE_BUS A4

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

void setup()
{
  // start serial port
  Serial.begin(115200);

  sensors.begin();
  
  if (sensors.isParasitePowerMode()) Serial.print("ON");
  else Serial.print("");
  

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
 
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{

  float tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  //Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(",");
}
void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer); 
  perhydro();
  ultrasonic();
  turbidity();
  totaldissolve();
  totaldo();

  delay(UPDATE_INTERVAL);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
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
  //Serial.print("Distance: ");
  Serial.print(dis1);
  Serial.print(",");
  //Serial.println("inches");
}

void turbidity(){
  float turbidityValue = analogRead(turbiditySensorPin);
  
  // Convert the raw value to turbidity in NTUs
  float turbidity = map(turbidityValue, 0,750,100,0);
  
  // Print the turbidity value to the serial monitor
  //Serial.print("Turbidity: ");
  Serial.print(turbidity);
  Serial.print(",");
  //Serial.println(turbidityValue);
  //Serial.println(" NTU");
 }

 void totaldissolve(){
  sensorValue = analogRead(tdssensorPin);
    Voltage = sensorValue*5/1024.0; //Convert analog reading to Voltage
    tdsValue=(133.42/Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
    //Serial.print("TDS Value = "); 
    Serial.println(tdsValue);
    //Serial.println(" ppm");
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
  //Serial.print("pH: ");
  Serial.print(pH);
  Serial.print(",");
 }

 void totaldo(){
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
 
  Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
  Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
  Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
  Serial.println("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");
  }
