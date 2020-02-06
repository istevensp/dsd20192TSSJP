//Inclusion de librerias y definicion de constantes
#include <ArduinoJson.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#define SOP '<'
#define EOP '>'
#define FIREBASE_HOST "dsd-agriculture.firebaseio.com" //sitio para alojar la base de datos en tiempo real
#define FIREBASE_AUTH "HAPVhxrHM25HgJalMgmAzPkrcbPqzCfA1iS91VGC" //ficha secreta de autenticacion generada de manera heredada por Firebase
#define WIFI_SSID "Lab-Telematica" //secuencia identificadora de la red inalambrica
#define WIFI_PASSWORD "l4bt3l3m4tic@" //clave de dicha red

//Uso de librerias y variables globales
SoftwareSerial SUART(4, 5); //SRX=Dpin-D2; STX-DPin-D1
FirebaseData firebaseData;
FirebaseJson json;
bool started = false;
bool ended = false;
char line[80];
byte indice;
const String path = "prueba"; //referencia a la llave de Firebase
int rangeRain=0;
double humidity=0.0, temperature=0.0, soilHumidity=0.0, ldrLux=0.0;

//configuracion de librerias a tasas de transmicion altas e incio de comunicacion inalambrica con la red y la plataforma de Google
void setup() {
  Serial.begin(115200);
  SUART.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado a: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

//ciclo infinito
void loop() {
  // Leyendo la informacion por serial entre ambas placas
  while(SUART.available() > 0){
    char inChar = SUART.read();
    if(inChar == SOP){ //validando el inicio del arreglo de la forma: <data>
      Serial.println("inicio del arreglo");
      indice = 0;
      line[indice] = '\0';
      started = true;
      ended = false;
    }
    else if(inChar == EOP){ //validando el fin del arreglo
      Serial.println("fin del arreglo");
      ended = true;
      Serial.println(line);
      Serial.println("extrayendo valores a nuevas variables locales");
      char *field;
      field = strtok( line, "," ); // separando el arreglo por comas
      humidity = atof( field ); //transformando lo extraido a tipo flotante
      field = strtok( nullptr, "," ); // redireccionando el resto del arreglo
      temperature = atof( field );
      field = strtok( nullptr, "," );
      soilHumidity = atof( field );
      field = strtok( nullptr, "," );
      rangeRain = atoi( field );
      field = strtok( nullptr, "," );
      ldrLux = atof( field );
      
      Serial.println("armando estructura Json");
      json.set("Humedad", humidity); //estableciendo las claves y valores de la coleccion Json
      json.set("Temperatura", temperature);
      json.set("HumedadSuelo", soilHumidity);
      json.set("Lluvia", rangeRain);
      json.set("Luz", ldrLux);
      if (Firebase.setJSON(firebaseData, path, json)){ //estableciendo la estructura en la base
        Serial.println("push correcto");
        String jsonStr;
        json.toString(jsonStr, true); //obteniendo el objeto JSON a manera de cadena
        Serial.println(jsonStr);
      }
      break;
    }
    else{
      if(indice < 79){//validacion de numero de caracteres en el arreglo del Serial
        line[indice] = inChar;
        indice++;
        line[indice] = '\0';
      }
    }
  }
  delay(25000);
}
