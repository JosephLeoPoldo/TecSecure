#include <Adafruit_Fingerprint.h>

SoftwareSerial SERIE(2,3);

Adafruit_Fingerprint DEDO = Adafruit_Fingerprint(&SERIE);

uint8_t id;

void setup() {
Serial.begin(9600);
DEDO.begin(57600);
/*
La función verifyPassword Verifica la contraseña de acceso de los sensores (la contraseña predeterminada es 0x0000000).
La función retorna true si la contraseña es correcta.
*/
if(DEDO.verifyPassword()){
Serial.println("Sensor de huella encontrado.");
}else{
Serial.println("No se encontró un sensor de huella");
while(1){delay(1);}
}
}

uint8_t leeNumero(void) {
uint8_t num = 0;
while(!num){ //Se queda en el while siempre y cuando num sea igual a cero.
while(!Serial.available());
num = Serial.parseInt();
}
return num;
}

void loop() {
Serial.println("Listo para registrar una huella digital.");
Serial.println("Escriba el número de ID (del 1 al 162) que desea guardar en este dedo.");
id = leeNumero();
if(!id){ //El id tiene que ser distinto de cero.
return;
}
Serial.print("Inscripción del ID ");
Serial.println(id);
registrarHuella();
while(1);
}

uint8_t registrarHuella() {
int p = -1;
Serial.print("Ingrese su dedo para poder registrarlo en el ID "); Serial.println(id);
while(p != FINGERPRINT_OK){
/*
La función getImage() pide al sensor que tome una imagen del dedo presionado en la superficie.
La función retorna:
FINGERPRINT_OK en caso de éxito.
FINGERPRINT_NOFINGER si no se detecta ningún dedo.
FINGERPRINT_PACKETRECIEVERR en error de comunicación.
FINGERPRINT_IMAGEFAIL en error de imagen.
*/
p = DEDO.getImage();
switch(p){
case FINGERPRINT_OK: Serial.println("Imagen tomada."); break;
case FINGERPRINT_NOFINGER: Serial.println("."); break;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); break;
case FINGERPRINT_IMAGEFAIL: Serial.println("Error de imagen."); break;
default: Serial.println("Error desconocido."); break;
}
}
/*
La función image2Tz() pide al sensor que convierta la imagen en una plantilla.
Recibe como parámetro la ubicación de la ranura para colocar la plantilla de 
características (se coloca una en 1 y otra en 2 para verificación para crear el modelo).
La función retorna:
FINGERPRINT_OK en caso de éxito.
FINGERPRINT_IMAGEMESS si la imagen está demasiado desordenada.
FINGERPRINT_PACKETRECIEVERR en error de comunicación.
FINGERPRINT_FEATUREFAIL al no poder identificar las características de las huellas dactilares.
FINGERPRINT_INVALIDIMAGE al no poder identificar las características de las huellas dactilares.
*/
p = DEDO.image2Tz(1);
switch(p){
case FINGERPRINT_OK: Serial.println("Imagen convertida."); break;
case FINGERPRINT_IMAGEMESS: Serial.println("Imagen demasiado desordenada."); return p;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); return p;
case FINGERPRINT_FEATUREFAIL: Serial.println("No se pudieron encontrar las características de la huella digital."); return p;
case FINGERPRINT_INVALIDIMAGE: Serial.println("No se pudieron encontrar las características de la huella digital."); return p;
default: Serial.println("Error desconocido."); return p;
}
Serial.println("Quite el dedo.");
delay(2000);
p = 0;
while(p != FINGERPRINT_NOFINGER){ //Se queda en el while siempre y cuando no se retire el dedo del sensor.
p = DEDO.getImage(); //Obtiene la imagen de la huella digital.
}
Serial.print("ID "); Serial.println(id);
p = -1;
Serial.println("Vuelva a colocar el mismo dedo.");
while(p != FINGERPRINT_OK){ //Se queda en el while si no se coloca el dedo.
p = DEDO.getImage(); //Obtiene la imagen de la huella digital.
switch(p){
case FINGERPRINT_OK: Serial.println("Imagen tomada"); break;
case FINGERPRINT_NOFINGER: Serial.print("."); break;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); break;
case FINGERPRINT_IMAGEFAIL: Serial.println("Error de imagen."); break;
default: Serial.println("Error desconocido."); break;
}
}
p = DEDO.image2Tz(2); //Pide al sensor que convierta la imagen en una plantilla.
switch(p){
case FINGERPRINT_OK: Serial.println("Imagen convertida."); break;
case FINGERPRINT_IMAGEMESS: Serial.println("Imagen demasiado desordenada."); return p;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); return p;
case FINGERPRINT_FEATUREFAIL: Serial.println("No se pudieron encontrar las características de la huella digital."); return p;
case FINGERPRINT_INVALIDIMAGE: Serial.println("No se pudieron encontrar las características de la huella digital."); return p;
default: Serial.println("Error desconocido."); return p;
}
Serial.print("Creando modelo para el id ");  Serial.println(id);
/*
La función createModel pide al sensor que tome dos plantillas de funciones de impresión y cree un modelo.
La función retorna:
FINGERPRINT_OK en caso de éxito.
FINGERPRINT_PACKETRECIEVERR en error de comunicación.
FINGERPRINT_ENROLLMISMATCH en discrepancia de huellas dactilares.
*/
p = DEDO.createModel();
switch(p){
case FINGERPRINT_OK: Serial.println("Impresiones emparejadas"); break;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación"); return p;
case FINGERPRINT_ENROLLMISMATCH: Serial.println("Las huellas dactilares no coinciden."); return p;
default: Serial.println("Error desconocido."); return p;
}
Serial.print("ID "); Serial.println(id);
/*
La función storeModel pide al sensor que almacene el modelo calculado para su posterior comparación.
La función recibe como parámetro la ubicación del id.
La función retorna:
FINGERPRINT_OK en caso de éxito.
FINGERPRINT_BADLOCATION si la ubicación no es válida.
FINGERPRINT_FLASHERR si el modelo no se pudo escribir en la memoria flash.
FINGERPRINT_PACKETRECIEVERR en error de comunicación.
*/
p = DEDO.storeModel(id);
switch(p){
case FINGERPRINT_OK: Serial.println("Huella almacenada."); break;
case FINGERPRINT_PACKETRECIEVEERR: Serial.println("Error de comunicación."); return p;
case FINGERPRINT_BADLOCATION: Serial.println("No se pudo almacenar en esa ubicación."); return p;
case FINGERPRINT_FLASHERR: Serial.println("Error de escritura en la memoria flash."); return p;
default: Serial.println("Error desconocido."); return p;
}
}
