const int relayPin = 7; // Pin GPIO al que está conectado el relé

void setup() {
  // Configurar el pin del relé como salida
  pinMode(relayPin, OUTPUT);
  // Inicialmente apagar el relé
  digitalWrite(relayPin, HIGH);
  
  // Inicializar la comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    // Leer los datos recibidos
    String command = Serial.readStringUntil('\n');
    command.trim(); // Eliminar espacios en blanco y saltos de línea

    // Comparar el comando recibido y actuar en consecuencia
    if (command.equalsIgnoreCase("ABRIR")) {
      digitalWrite(relayPin, LOW); // Encender el relé
      Serial.println("Relé encendido");
      delay(3000); // Esperar 3 segundos
      digitalWrite(relayPin, HIGH); // Apagar el relé
      Serial.println("Relé apagado");
    } else {
      Serial.println("Comando desconocido");
    }
  }
}
