#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

//dht11
#define DHTPIN 19
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//credenciales del wifi
const char* ssid = "Yohan :)";
const char* password = "pandita19";

//credenciales de adafruit io
const char* aio_username = "tu_username";
const char* aio_key = "tu_key";


void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectandose...");
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n¡Conexion WiFi exitosa!");
  Serial.print("IP: "); Serial.print(WiFi.localIP()); 
}

void loop() {
  if(WiFi.status()==WL_CONNECTED){
    float t= dht.readTemperature();
    float f= dht.readTemperature(true);
    float h= dht.readHumidity();

    if(isnan(h) || isnan(t) || isnan(f)){
      Serial.print("Error en el sensor DHT11!!");
      delay(2000);
      return;
    }

    //enviar al Adafruit IO
    enviarDatos("temperatura", t);
    enviarDatos("humedad", h);
    enviarDatos("temperatura_f", f);

    Serial.printf("Datos enviados → TempC: %.1f°C | TemF: %.1f°F | Hum: %.1f%%\n", t, f, h);
  }else{
    Serial.println("Wifi desconectado, Intentando reconectar....");
    WiFi.reconnect();
  }

  delay(8000);
}

void enviarDatos(const char* feed, float valor){
  HTTPClient http;

  String url= "https://io.adafruit.com/api/v2/";
  url += aio_username;
  url += "/feeds/";
  url += feed;
  url += "/data";

  http.begin(url);
  http.addHeader("Content-Type","application/json");
  http.addHeader("X-AIO-Key", aio_key);

  String payload = "{\"value\":\"" + String(valor) +"\"}";
  int httpCode = http.POST(payload);

  if(httpCode == HTTP_CODE_OK){
    Serial.print(feed); Serial.println(": OK");
  }else{ 
    Serial.printf("Error %d en %s\n", httpCode, feed);
  }

  http.end();

}
