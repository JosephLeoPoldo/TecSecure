#include <Adafruit_Fingerprint.h>

SoftwareSerial SERIE(2,3);

Adafruit_Fingerprint DEDO = Adafruit_Fingerprint(&SERIE);

void setup() {
Serial.begin(9600);
Serial.println("Elimina huellas del sensor.");
DEDO.begin(57600);  
if(DEDO.verifyPassword()){
Serial.println("Bucando sensor de huellas...");
}else{
Serial.println("No se detectó ningún sensor...");
while(1);
}
}

uint8_t leeNumero(void) {
uint8_t num = 0;
while(!num){
while(!Serial.available());
num = Serial.parseInt();
}
return num;
}

void loop() {
Serial.println("Escriba el número de identificación (del 1 al 162) que desea eliminar...");
uint8_t id = leeNumero();
if(!id){//El ID = 0 no está permitido.
return;
}
Serial.print("Borrando ID #");
Serial.println(id);
borraHuella(id);
while(1);
}

uint8_t borraHuella(uint8_t id) {
uint8_t p = -1;
/*
La función deleteModel pide al sensor que elimine un modelo en la memoria.
La función recibe como parámetro la ubicación del id.
La función retorna:
FINGERPRINT_OK en caso de éxito.
FINGERPRINT_BADLOCATION si la ubicación no es válida.
FINGERPRINT_FLASHERR si el modelo no se pudo escribir en la memoria flash.
FINGERPRINT_PACKETRECIEVERR en error de comunicación.
*/
p = DEDO.deleteModel(id);
switch(p)
{
case FINGERPRINT_OK: Serial.println("Huella borrada."); return p;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); return p;
case FINGERPRINT_BADLOCATION: Serial.println("No se pudo eliminar en esa ubicación."); return p;
case FINGERPRINT_FLASHERR: Serial.println("Error escribiendo en la memoria flash"); return p;
default: Serial.print("Error desconocido: 0x"); Serial.println(p, HEX); return p;
}
}
