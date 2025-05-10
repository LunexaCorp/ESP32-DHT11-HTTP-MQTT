#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

//PinOut DHT11
#define DHT_PIN   19
#define DHT_TYPE  DHT11
DHT dht(DHT_PIN, DHT_TYPE);

//Credenciales de red WiFi
#define WIFI_SSID  "Yohan :)"
#define WIFI_PSWD  "pandita19"

//Credenciales del Adafruit IO
#define IO_USERNAME    "tu_username"
#define IO_KEY         "tu_key"

// Broker MQTT de Adafruit IO
const char *mqtt_broker   = "io.adafruit.com";
const int   mqtt_port     = 1883;
const char *client_id     = "esp32-client";

//Topic MQTT
const char *topic_temp = "parxita/feeds/temperatura";
const char *topic_hume = "parxita/feeds/humedad";

//Prototipos de funciones
void wifiConnect (void);
void mqttConnect (void);

//Objeto para manejar conexion IP para el ESP32
WiFiClient espClient;
//Objeto para manejar las conexiones y transferencias mediante MQTT
PubSubClient clientMqtt(espClient);

//Variables de aplicacion
float tempC, tempF, humedad;
char tempString[10];
char humString[10];
char dataJSON[100];

void setup() {
  Serial.begin(115200);
  //Configuracion del sensor DHT11
  dht.begin();
  //Conexion a WiFi en modo estacion
  wifiConnect();
  //Conexion MQTT
  mqttConnect();
}

void loop() {
  //Lectura del sensor DHT11
  tempC = dht.readTemperature();     // °C
  tempF = dht.readTemperature(true); // °F
  humedad = dht.readHumidity();      // %

  if(WL_CONNECTED == WiFi.status()) {
    //Comunicacion MQTT
    if(true == clientMqtt.connected()) {

      // Convertir valores float a string
      dtostrf(tempC, 6, 2, tempString); // 6 dígitos, 2 decimales
      dtostrf(humedad, 6, 2, humString);

      // Publicar cada uno en su feed
      clientMqtt.publish(topic_temp, tempString);
      clientMqtt.publish(topic_hume, humString);

      //Formatear los datos en JSON para Publicar en el broker
      sprintf(dataJSON, "{\"temperatura_c\":%.2f,\"temperatura_f\":%.2f,\"humedad\":%.2f}", tempC, tempF, humedad);
      Serial.printf("DATA PUB: %s \n\n", dataJSON);

    }
    else {
      //Reconexion MQTT
      mqttConnect();
    }
    //Mantener la conexion actualizada
    clientMqtt.loop();
  }
  else {
    //Reconexion WiFi
    wifiConnect();
  }
  delay(10000);
}

//****************************************************************************************
//************************DESAROLLO DE FUNCIONES******************************************

void wifiConnect (void) {
  //Configuracion de WiFi en modo estacion
  WiFi.mode(WIFI_STA);
  //Establecer el SSID y PSWD de la red WiFi
  WiFi.begin(WIFI_SSID, WIFI_PSWD);
  Serial.print("ESP32 Conectando a red WiFi");
  //Consultar el estado de conextion
  while(WL_CONNECTED != WiFi.status()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("ESP32 Conexion Lista!!!");
  Serial.println(" ");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print(" Access Point: ");
  Serial.print(WiFi.SSID());
  Serial.print(" - ");
  Serial.println(WiFi.RSSI());
}

//Función para conectar a MQTT (Adafruit IO)
void mqttConnect(void){
  clientMqtt.setServer(mqtt_broker, mqtt_port);
  
  while(!clientMqtt.connected()){
    Serial.print("Intentando conexión MQTT...");

    //Intentar conectar con el nombre de cliente y credenciales
    if(clientMqtt.connect(client_id, IO_USERNAME, IO_KEY)){
      Serial.println("Conectando a MQTT");
    }else{
      Serial.print("Fallo en la conexión. Estado: ");
      Serial.println(clientMqtt.state());
      delay(2000);
    }
  }
}



