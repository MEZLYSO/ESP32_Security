from datetime import datetime
import cv2
import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
TOPIC = "casa/puerta/estado"


def tomar_foto():
  cap = cv2.VideoCapture(0)  
  if cap.isOpened():
    ret, frame = cap.read()
    if ret:
      timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
      filename = f"captura_{timestamp}.jpg"
      cv2.imwrite(filename, frame)
      print(f"[FOTO GUARDADA] {filename}")
    cap.release()
  else:
    print("[ERROR] No se pudo acceder a la cámara")


def on_message(client, userdata, msg):
  payload = msg.payload.decode("utf-8").strip()

  if payload == "abierto":
    print("¡Se detectó puerta abierta! Tomando foto...")
    tomar_foto()


client = mqtt.Client()
client.on_message = on_message

client.connect(BROKER, 1883, 60)
client.subscribe(TOPIC)

print("Escuchando MQTT...")
client.loop_forever()