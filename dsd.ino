//Inclusion de librerias y definicion de constantes
#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTPIN 7 
#define DHTTYPE DHT11
#define LDR_PIN A2 
#define MAX_ADC_READING 1023 
#define ADC_REF_VOLTAGE 5.0 
#define REF_RESISTANCE 4700 
#define LUX_CALC_SCALAR 12518931
#define LUX_CALC_EXPONENT -1.405

//variables globales y uso de librerias
const int sensorMin = 0; 
const int sensorMax = 1024;
int fc37 = A0; //rain sensor analog pin
int fc28 = A3; //soil humidity digital pin
DHT dht(DHTPIN, DHTTYPE); 
SoftwareSerial SUART(2, 3); //SRX=Dpin2, STX=Dpin3

//configuracion de librerias a tasas de transmicion y modos de pines
void setup() {
  Serial.begin(115200);
  SUART.begin(115200);
  dht.begin();
  pinMode(fc28, INPUT);
}

//ciclo infinito
void loop() {
  //variables y sus lecturas respectivas
  float resistorVoltage, ldrVoltage, ldrResistance, ldrLux;
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  
  float soilHumidity = analogRead(fc28);
  float rainVoltage = analogRead(fc37);
  int ldrRawData = analogRead(LDR_PIN);
  int rangeRain = map(rainVoltage, sensorMin, sensorMax, 0, 3);

  // Conversion ADC con voltaje referencial
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  
  // voltaje del ldr basado en resta de voltajes voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  // resistencia que el LDR tendria para dicho voltaje
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;

  // calculo de la luminancia
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  //imprimiendo por el puerto serial
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print(soilHumidity);
  Serial.print(", ");
  Serial.print(rangeRain);
  Serial.print(", ");
  Serial.print(ldrLux);
  Serial.println(); 

  //enviando por SUART al NodeMCU
  SUART.print('<');
  SUART.print(humidity, DEC);
  SUART.print(',');
  SUART.print(temperature, DEC);
  SUART.print(',');
  SUART.print(soilHumidity, DEC);
  SUART.print(',');
  SUART.print(rangeRain, DEC);
  SUART.print(',');
  SUART.print(ldrLux, DEC);
  SUART.print('>');
  SUART.println();
  
  delay(5000);
}
