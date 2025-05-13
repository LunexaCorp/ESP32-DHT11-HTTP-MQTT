# Proyecto ESP32 con Sensor DHT11

Este proyecto utiliza un **ESP32** para leer datos de un **sensor DHT11** de temperatura y humedad, y los envía a **Adafruit IO** utilizando dos tipos de conexión: **HTTP** y **MQTT**. El objetivo es recolectar datos de sensores y visualizarlos en una plataforma en la nube para monitoreo remoto.

---

## 📚 Descripción

El proyecto está dividido en dos secciones principales:

1. **Conexión HTTP**: El ESP32 envía los datos del sensor DHT11 a Adafruit IO mediante una petición HTTP.
2. **Conexión MQTT**: El ESP32 se conecta a Adafruit IO usando el protocolo MQTT para enviar los datos en tiempo real.

---

## 🧰 Componentes utilizados

- **ESP32** (NodeMCU 38P)
- **Sensor DHT11** (Temperatura y Humedad)
- **Plataforma Adafruit IO** (Para almacenar y visualizar los datos)
- **Protocolo HTTP y MQTT** para la transmisión de datos

---

## ✅ Requisitos

- **Arduino IDE** configurado para programar el ESP32.
- Una cuenta en **Adafruit IO**.
- Bibliotecas necesarias: `DHT`, `WiFi`, `PubSubClient`.
- Librería personalizada: [`UtilidadesClima`](https://github.com/LunexaCorp/UtilidadesClima)

---

## 📦 Librería UtilidadesClima

Este proyecto hace uso de la librería personalizada **[UtilidadesClima](https://github.com/LunexaCorp/UtilidadesClima)** para realizar cálculos climáticos a partir de los valores del sensor DHT11.

### Instalación de la librería

1. Descarga la librería desde GitHub:  
   [`https://github.com/LunexaCorp/UtilidadesClima`](https://github.com/LunexaCorp/UtilidadesClima)

2. Copia la carpeta `UtilidadesClima` en tu carpeta de librerías de Arduino:
   - **Windows:** `Documentos/Arduino/libraries/`
   - **macOS/Linux:** `~/Arduino/libraries/`

3. Reinicia el IDE de Arduino si estaba abierto.

---

## 🔒 Manejo seguro de credenciales

Este proyecto **no almacena credenciales directamente en el código fuente**. En su lugar, usa un archivo separado `secrets.h`, que **no debe ser incluido en Git**.

### Ejemplo de archivo `secrets.h`:

```cpp
#ifndef SECRETS_H
#define SECRETS_H

struct Secrets {
  static constexpr const char* WIFI_SSID   = "TuRedWiFi";
  static constexpr const char* WIFI_PSWD   = "TuPassword";
  static constexpr const char* IO_USERNAME = "tu_usuario";
  static constexpr const char* IO_KEY      = "aio_tuClaveGenerada";
};

#endif
