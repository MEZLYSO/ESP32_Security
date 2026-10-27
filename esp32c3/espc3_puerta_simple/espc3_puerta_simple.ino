const int sensorPin = 3;
const int ledPin = 4;  

void setup() {
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int estadoSensor = digitalRead(sensorPin);

  if (estadoSensor == LOW) { 
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(150);
}