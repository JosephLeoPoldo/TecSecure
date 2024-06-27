int ledPin = 8; // Pin del LED

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "on") {
      digitalWrite(ledPin, HIGH); // Encender LED
      delay(1000); // Mantener encendido por 1 segundo
      digitalWrite(ledPin, LOW); // Apagar LED
    }
  }
}