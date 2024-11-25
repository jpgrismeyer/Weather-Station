
#include "ThingSpeak.h"
#include "WiFi.h"

#include "DHT.h"
#include "Adafruit_BMP085.h"


#define pin1 13       //Pin del DHT11.


const char* ssid = "Galaxy A527ABC";                        //SSID de vuestro router.
const char* password = "hlmu6564";                //Contraseña de vuestro router.

unsigned long channelID = 2754006;                //ID de vuestro canal.
const char* WriteAPIKey = "FO3YJWNORQB4ZY29";     //Write API Key de vuestro canal.

WiFiClient cliente;

DHT dht1(pin1, DHT11);    //El azul.

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  Serial.println("Test de sensores:");

  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");

  ThingSpeak.begin(cliente);

  dht1.begin();
  bmp.begin();
}

void loop() {

  delay(2000);
  leerdht1();
  
  delay(2000);
  leerbmp();

  ThingSpeak.writeFields(channelID,WriteAPIKey);
  Serial.println("Datos enviados a ThingSpeak!");
  delay(14000);
}

void leerdht1() {
  
  float t1 = dht1.readTemperature();
  float h1 = dht1.readHumidity();

  while (isnan(t1) || isnan(h1)){
    Serial.println("Lectura fallida en el sensor DHT11, repitiendo lectura...");
    delay(2000);
    t1 = dht1.readTemperature();
    h1 = dht1.readHumidity();
  }

  Serial.print("Temperatura DHT11: ");
  Serial.print(t1);
  Serial.println(" ºC.");

  Serial.print("Humedad DHT11: ");
  Serial.print(h1);
  Serial.println(" %."); 

  Serial.println("-----------------------");

  ThingSpeak.setField (1,t1);
  ThingSpeak.setField (2,h1);
}

void leerbmp() {

  float presion = bmp.readPressure();
  float altitud = bmp.readAltitude();

  Serial.print("Presión bmp: ");
  Serial.print(presion);
  Serial.println(" Pa.");

  Serial.print("Altitud bmp: ");
  Serial.print(altitud);
  Serial.println(" metros.");

  Serial.println("-----------------------");  

  ThingSpeak.setField (3,presion);
  ThingSpeak.setField (4,altitud);
}