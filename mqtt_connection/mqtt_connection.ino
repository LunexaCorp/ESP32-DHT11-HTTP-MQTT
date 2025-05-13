#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <UtilidadesClima.h>
#include "../include/secrets.h"

//Configuración del sensor 
#define DHT_PIN   19
#define DHT_TYPE  DHT11
DHT dht(DHT_PIN, DHT_TYPE);

//Credenciales de red WiFi
const char* WIFI_SSID = Secrets::WIFI_SSID;
const char* WIFI_PSWD = Secrets::WIFI_PSWD;

//Datos de Adafruit IO
const char* IO_USERNAME = Secrets::IO_USERNAME;
const char* IO_KEY = Secrets::IO_KEY;
const char* MQTT_BROKER = "io.adafruit.com";
const int   MQTT_PORT = 1883;
const char *CLIENT_ID = "esp32-client";

// Feeds (Temas MQTT)
const char* TOPIC_CLIMA_TEMP_C        = "parxita/feeds/clima.ambiente.temperatura_celsius";
const char* TOPIC_CLIMA_TEMP_F        = "parxita/feeds/clima.ambiente.temperatura_fahrenheit";
const char* TOPIC_CLIMA_TEMP_K        = "parxita/feeds/clima.ambiente.temperatura_kelvin";
const char* TOPIC_CLIMA_TEMP_R        = "parxita/feeds/clima.ambiente.temperatura_rankine";
const char* TOPIC_CLIMA_HUMEDAD_R       = "parxita/feeds/clima.ambiente.humedad_relativa";
const char* TOPIC_CLIMA_HUMEDAD_A       = "parxita/feeds/clima.ambiente.humedad_absoluta";
const char* TOPIC_CLIMA_PUNTO_ROCIO   = "parxita/feeds/clima.ambiente.punto_rocio_celsius";
const char* TOPIC_CLIMA_SENS_TERM     = "parxita/feeds/clima.ambiente.sensacion_termica_celsius";


//Objetos de conexión
//Objeto para manejar conexion IP para el ESP32
WiFiClient wifiClient;
//Objeto para manejar las conexiones y transferencias mediante MQTT
PubSubClient mqttClient(wifiClient);

//Funciones auxiliares
void connectToWiFi();
void connectToMQTT();
void publishSensorData();
void readAndSendDHTData();

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();
  connectToMQTT();
}

void loop() {

  if(WiFi.status() != WL_CONNECTED)
    connectToWiFi();

  if(!mqttClient.connected())
    connectToMQTT();

  mqttClient.loop();    // Mantener conexión MQTT
  publishSensorData();  // Leer sensor y publicar datos

  delay(18000);         // Esperar 18 segundos

}

//****************************************************************************************
//************************DESAROLLO DE FUNCIONES******************************************

void connectToWiFi() {
  Serial.print("Conectando a WiFi...");
  //Establecer el SSID y PSWD de la red WiFi
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  //Configuracion de WiFi en modo estacion
  WiFi.mode(WIFI_STA);
  
  //Consultar el estado de conexión
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print(" Access Point: ");
  Serial.print(WiFi.SSID());
  Serial.print(" - ");
  Serial.println(WiFi.RSSI());
}

//Función para conectar a MQTT (Adafruit IO)
void connectToMQTT(){
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  
  while(!mqttClient.connected()){
    Serial.print("Conectando a MQTT...");

    //Intentar conectar con el nombre de cliente y credenciales
    if(mqttClient.connect(CLIENT_ID, IO_USERNAME, IO_KEY)){
      Serial.println("¡conectado!");
    }else{
      Serial.print(" Error (Estado: ");
      Serial.print(mqttClient.state());
      Serial.println("). Reintentando...");
      delay(2000);
    }
  }
}

void publishSensorData(){
  float temperaturaCelsius = dht.readTemperature();
  float humedadRelativa = dht.readHumidity();
  

  if(isnan(temperaturaCelsius) || isnan(humedadRelativa)) {
    Serial.println("Error al leer el sensor");
    return;
  }

  float puntoRocio = calcularPuntoRocio(temperaturaCelsius, humedadRelativa);
  float sensacionTermica = calcularSensacionTermica(temperaturaCelsius, humedadRelativa);
  String temperaturaFahrenheit = convertirDeCelsiusA(FAHRENHEIT, false, temperaturaCelsius);
  String temperaturaKelvin     = convertirDeCelsiusA(KELVIN, false, temperaturaCelsius);
  String temperaturaRankine    = convertirDeCelsiusA(RANKINE, false, temperaturaCelsius);
  float humedadAbsoluta = calcularHumedadAbsoluta(temperaturaCelsius, humedadRelativa);

  char tempCStr[8], tempFStr[8], tempKStr[8], tempRStr[8];
  char humedadRStr[8], puntoRocioStr[8], sensTermStr[8];
  char humedadAStr[8];


  // Convertir los datos float a cadenas de texto con 2 decimales
  dtostrf(temperaturaCelsius, 4, 2, tempCStr);
  temperaturaFahrenheit.toCharArray(tempFStr, sizeof(tempFStr));
  temperaturaKelvin.toCharArray(tempKStr, sizeof(tempKStr));
  temperaturaRankine.toCharArray(tempRStr, sizeof(tempRStr));
  dtostrf(humedadRelativa, 4, 2, humedadRStr);
  dtostrf(humedadAbsoluta, 4, 2, humedadAStr);
  dtostrf(puntoRocio, 4, 2, puntoRocioStr);
  dtostrf(sensacionTermica, 4, 2, sensTermStr);

  // Publicar datos a los feeds MQTT respectivos
  mqttClient.publish(TOPIC_CLIMA_TEMP_C, tempCStr);
  mqttClient.publish(TOPIC_CLIMA_TEMP_F, tempFStr);
  mqttClient.publish(TOPIC_CLIMA_TEMP_K, tempKStr);
  mqttClient.publish(TOPIC_CLIMA_TEMP_R, tempRStr);
  mqttClient.publish(TOPIC_CLIMA_HUMEDAD_R, humedadRStr);
  mqttClient.publish(TOPIC_CLIMA_HUMEDAD_A, humedadAStr);
  mqttClient.publish(TOPIC_CLIMA_PUNTO_ROCIO, puntoRocioStr);
  mqttClient.publish(TOPIC_CLIMA_SENS_TERM, sensTermStr);

  // Mensaje de confirmación por consola
  Serial.println("📡 Datos enviados vía MQTT:");
  Serial.printf("  - Temp: %s °C | %s °F | %s K | %s R\n", tempCStr, tempFStr, tempKStr, tempRStr);
  Serial.printf("  - Humedad relativa: %s %%\n", humedadRStr);
  Serial.printf("  - Humedad absoluta: %s %%\n", humedadAStr);
  Serial.printf("  - Punto de rocío: %s °C\n", puntoRocioStr);
  Serial.printf("  - Sensación térmica: %s °C\n", sensTermStr);
  
}