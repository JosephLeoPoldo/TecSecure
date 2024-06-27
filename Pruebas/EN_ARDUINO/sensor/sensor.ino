#include <Adafruit_Fingerprint.h>

SoftwareSerial SERIE(2, 3);

Adafruit_Fingerprint DEDO = Adafruit_Fingerprint(&SERIE);

void setup() {
  Serial.begin(9600);
  DEDO.begin(57600);
  if (DEDO.verifyPassword()) {
    Serial.println("Sensor de huella encontrado.");
  } else {
    Serial.println("No se encontró un sensor de huella.");
    while (1) {
      delay(1);
    }
  }

  DEDO.getTemplateCount();
  Serial.print("EL sensor contiene ");
  Serial.print(DEDO.templateCount);
  Serial.println(" plantilla/as");
  Serial.println("Esperando el ingreso de una huella válida...");
}

void loop() {
  int id = detectaHuella();
  if (id > 0) {
    enviarIDMQTT(id);
  } 
  //else if (id == -1) {
    //Serial.println("Dedo no registrado.");
    //enviarIDMQTT(id);
  //}
  delay(2000);
}

int detectaHuella() {
  uint8_t p = DEDO.getImage();
  if (p != FINGERPRINT_OK) return -2;
  p = DEDO.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  p = DEDO.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  Serial.print("ID encontrado #");
  Serial.print(DEDO.fingerID);
  Serial.print(" con una confianza de ");
  Serial.println(DEDO.confidence);
  return DEDO.fingerID;
}

void enviarIDMQTT(int id) {
  Serial.print("MQTT:");
  Serial.println(id);
}
