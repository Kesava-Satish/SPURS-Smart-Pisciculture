#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A4

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

void setup()
{
  // start serial port
  Serial.begin(9600);

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
  Serial.print("Temp C: ");
  Serial.println(tempC);
}
void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer); 
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
