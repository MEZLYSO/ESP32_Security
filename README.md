# ESP32 Security

## Proyectos

### ESP32-C3 - Puerta simple (`esp32c3/espc3_puerta_simple/`)

Sistema de monitoreo de puerta con ESP32-C3 que detecta el estado de una puerta mediante un sensor (reed switch / sensor de contacto) y reporta el cambio de estado a través de MQTT.

### Servidor MQTT - Captura de fotos (`server/espc3_puerta_simple_server/`)

Servidor en Python que se suscribe al tópico MQTT del sistema de puerta y toma una fotografía con la cámara del equipo cuando se detecta que la puerta ha sido abierta.

#### Características

- **Detección de estado de puerta**: Usa un sensor conectado al GPIO 4 (con `INPUT_PULLUP`). Se considera `CERRADO` cuando el sensor está en `LOW` (contacto cerrado) y `ABIERTO` cuando está en `HIGH` (contacto abierto).
- **Indicador visual**: LED en GPIO 8 que se enciende cuando la puerta está abierta (`HIGH`).
- **Conectividad WiFi**: Se conecta automáticamente a la red configurada al arrancar.
- **Integración MQTT**: Publica cambios de estado en el broker `broker.hivemq.com:1883` en el topic `casa/puerta/estado` con los mensajes `abierto` o `cerrado`.
- **Reconexión automática**: Reintenta reconectar tanto a WiFi (en `conectarWiFi()`) como a MQTT (en `reconnect()`) cuando hay desconexiones.
- **Publicación por cambio de estado**: Solo publica cuando hay un cambio en el estado del sensor (evita publicaciones redundantes).

#### Hardware

| Componente | Pin ESP32-C3 | Notas |
|---|---|---|
| Sensor de puerta (reed switch / contacto) | GPIO 4 | Configurado con `INPUT_PULLUP`. Lógica inversa según montaje. |
| LED indicador | GPIO 8 | Indicador visual de puerta abierta. |

#### Software

##### Dependencias

- [WiFi.h](https://github.com/espressif/arduino-esp32) - Conectividad WiFi para ESP32
- [PubSubClient.h](https://github.com/knolleary/pubsubclient) - Cliente MQTT

##### Configuración

Editar las credenciales y parámetros en `esp32c3/espc3_puerta_simple/espc3_puerta_simple.ino`:

```cpp
const char* ssid = "TU_RED_WIFI";
const char* password = "TU_CONTRASEÑA";
const char* mqtt_server = "broker.hivemq.com";
```

##### Tópicos MQTT

| Tópico | Valores | Descripción |
|---|---|---|
| `casa/puerta/estado` | `abierto`, `cerrado` | Estado actual de la puerta. Se publica únicamente en cambio de estado. |

## Servidor - Detalles

### Características

- **Suscripción MQTT**: Se suscribe al broker `broker.hivemq.com:1883` en el tópico `casa/puerta/estado`.
- **Captura automática**: Al recibir el mensaje `abierto`, toma una foto con la cámara local (`cv2.VideoCapture(0)`).
- **Guardado con timestamp**: Guarda las imágenes como `captura_YYYYMMDD_HHMMSS.jpg`.
- **Registro en consola**: Imprime el estado de la captura y errores si no hay cámara disponible.

### Dependencias

- [paho-mqtt](https://pypi.org/project/paho-mqtt/) - Cliente MQTT para Python
- [opencv-python](https://pypi.org/project/opencv-python/) - Captura y guardado de imágenes desde la cámara

### Ejecución

```bash
pip install paho-mqtt opencv-python
python server/espc3_puerta_simple_server/main.py
```

