#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// Crear el objeto lcd con dirección 0x27 y dimensiones de 16 columnas x 2 filas
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7

const int relayPin = 7; // Pin GPIO al que está conectado el relé
const int ledPin = 8; // Pin del LED

void setup() {
    // Inicializar la comunicación serial a 9600 baudios
    Serial.begin(9600);      

    // Inicializar la pantalla LCD
    lcd.setBacklightPin(3, POSITIVE); // Puerto P3 de PCF8574 como positivo
    lcd.setBacklight(HIGH);           // Habilita iluminación posterior de LCD
    lcd.begin(16, 2);                 // 16 columnas por 2 líneas para LCD 1602A
    lcd.clear();                      // Limpia la pantalla

    // Configurar el pin del relé como salida y apagarlo inicialmente
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);

    // Configurar el pin del LED como salida
    pinMode(ledPin, OUTPUT);
}

void loop() {
    // Verificar si hay datos disponibles en el puerto serial
    if (Serial.available() > 0) {
        // Leer la línea de datos recibidos hasta encontrar un salto de línea
        String message = Serial.readStringUntil('\n');
        message.trim(); // Eliminar espacios en blanco y saltos de línea

        // Comparar el comando recibido y actuar en consecuencia
        if (message.equalsIgnoreCase("ON") || message.equalsIgnoreCase("OFF") || message.equalsIgnoreCase("ABRIR")) {
            Serial.println("Comando conocido recibido");
            
            // Controlar el relé con el comando "ABRIR"
            if (message.equalsIgnoreCase("ABRIR")) {
                digitalWrite(relayPin, LOW); // Encender el relé
                Serial.println("Relé encendido");
                delay(3000); // Esperar 3 segundos
                digitalWrite(relayPin, HIGH); // Apagar el relé
                Serial.println("Relé apagado");
            }
            
            // Controlar el LED con el comando "ON"
            if (message.equalsIgnoreCase("ON")) {
                digitalWrite(ledPin, HIGH); // Encender LED
                delay(1000); // Mantener encendido por 1 segundo
                digitalWrite(ledPin, LOW); // Apagar LED
                Serial.println("LED apagado");
            }

        } else {
            Serial.println("Datos recibidos: " + message);  // Imprime el mensaje recibido en el monitor serial

            // Limpiar la pantalla LCD
            lcd.clear();
            
            // Mostrar un mensaje fijo en la primera línea
            lcd.setCursor(0, 0);
            lcd.print("Mensaje recibido:");
            
            // Mostrar el mensaje recibido en la segunda línea
            lcd.setCursor(0, 1);
            lcd.print(message);

            // Esperar 3 segundos antes de limpiar la pantalla LCD
            delay(3000);
            lcd.clear();
        }
    }
}
