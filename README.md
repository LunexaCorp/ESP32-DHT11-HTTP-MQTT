# Proyecto ESP32 con Sensor DHT11

Este proyecto utiliza un **ESP32** para leer datos de un **sensor DHT11** de temperatura y humedad, y los envía a **Adafruit IO** utilizando dos tipos de conexión: **HTTP** y **MQTT**. El objetivo es recolectar datos de sensores y visualizarlos en una plataforma en la nube para monitoreo remoto.

## Descripción

El proyecto está dividido en dos secciones principales:
1. **Conexión HTTP**: El ESP32 envía los datos del sensor DHT11 a Adafruit IO mediante una petición HTTP.
2. **Conexión MQTT**: El ESP32 se conecta a Adafruit IO usando el protocolo MQTT para enviar los datos en tiempo real.

## Componentes utilizados

- **ESP32** (NodeMCU 38P)
- **Sensor DHT11** (Temperatura y Humedad)
- **Plataforma Adafruit IO** (Para almacenar y visualizar los datos)
- **Protocolo HTTP y MQTT** para la transmisión de datos

## Instrucciones

### Requisitos
- **Arduino IDE** configurado para programar el ESP32.
- Una cuenta en **Adafruit IO**.
- Bibliotecas necesarias: `DHT`, `WiFi`, `AdafruitIO`.

### Pasos

1. **Configura tu Adafruit IO**:
   - Crea una cuenta en Adafruit IO y genera un **AIO Key**.
   - Crea un **feed** para temperatura y otro para humedad.

2. **Configura el código**:
   - Abre el archivo `.ino` en el Arduino IDE.
   - Sustituye tu **AIO Key** y las credenciales WiFi en el código.

3. **Carga el código al ESP32**.

4. **Monitorea los datos**:
   - Los datos del sensor aparecerán en tu **Dashboard de Adafruit IO**.