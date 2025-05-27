# Practica 6
## Buses de comunicación II (SPI)
El objetivo de esta práctica es entender el funcionamiento del bus de comunicación SPI (Serial Peripheral Interface). El bus SPI permite la comunicación Full Duplex entre un dispositivo maestro y uno o varios dispositivos esclavos.


#Ejercicio 1 LECTURA/ESCRITURA DE MEMORIA SD:
Codigo main.cpp:
```cpp
#include <SPI.h>
#include <SD.h>
File myFile;
void setup()
{
Serial.begin(9600);
Serial.print("Iniciando SD ...");
if (!SD.begin(4)) {
Serial.println("No se pudo inicializar");
return;
}
Serial.println("inicializacion exitosa");
myFile = SD.open("archivo.txt");//abrimos el archivo
if (myFile) {
Serial.println("archivo.txt:");
while (myFile.available()) {
Serial.write(myFile.read());
}
myFile.close(); //cerramos el archivo
} else {
Serial.println("Error al abrir el archivo");
}
}
void loop()
{
}
```
Este código lee el contenido de un archivo llamado archivo.txt desde una tarjeta SD y lo muestra en el monitor serial. Primero, inicia la comunicación serial e intenta inicializar la tarjeta SD usando el pin 4. Si la inicialización falla, muestra un mensaje de error; si tiene éxito, intenta abrir el archivo. Si el archivo se abre correctamente, lee su contenido y lo imprime por el serial. Al finalizar, cierra el archivo. La función loop() no se utiliza, ya que todo ocurre en el setup().


#Ejercicio 2 LECTURA DE ETIQUETA RFID:
Codigo main.cpp:
```cpp
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5   // Pin SDA del RC522 (conectado a GPIO5 de la ESP32-S3)
#define RST_PIN 4  // Pin RST del RC522 (conectado a GPIO4 de la ESP32-S3)
#define SCK_PIN 36 // Pin SCK del RC522 (conectado a GPIO36 de la ESP32-S3)
#define MOSI_PIN 35 // Pin MOSI del RC522 (conectado a GPIO35 de la ESP32-S3)
#define MISO_PIN 37 // Pin MISO del RC522 (conectado a GPIO37 de la ESP32-S3)

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(115200);
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); // Iniciar SPI con pines personalizados
    mfrc522.PCD_Init(); // Iniciar MFRC522
    Serial.println("Escanea una tarjeta RFID...");
    
    // Configurar clave de fábrica por defecto FF FF FF FF FF FF
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

void loop() {
    // Verifica si hay una tarjeta presente
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Intenta leer la tarjeta
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Mostrar UID
    Serial.print("UID de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Mostrar tipo de tarjeta
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.print("Tipo de tarjeta: ");
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Intentar autenticarse en el sector 0
    byte sector = 0;
    byte blockAddr = sector * 4 + 3; // Bloque de claves en el sector
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));
    if (status == MFRC522::STATUS_OK) {
        Serial.println("Autenticación exitosa en el sector 0");
        
        // Leer bloque de claves
        byte buffer[18];
        byte size = sizeof(buffer);
        status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
        if (status == MFRC522::STATUS_OK) {
            Serial.print("Clave A: ");
            for (byte i = 0; i < 6; i++) {
                Serial.print(buffer[i] < 0x10 ? " 0" : " ");
                Serial.print(buffer[i], HEX);
            }
            Serial.println();

            Serial.print("Clave B: ");
            for (byte i = 10; i < 16; i++) {
                Serial.print(buffer[i] < 0x10 ? " 0" : " ");
                Serial.print(buffer[i], HEX);
            }
            Serial.println();
        } else {
            Serial.println("Error al leer el bloque de claves");
        }
    } else {
        Serial.println("Fallo en la autenticación del sector 0");
    }

    // Detener comunicación con la tarjeta
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
```


Este código está diseñado para usar un lector RFID RC522 con una ESP32-S3, permitiendo leer tarjetas RFID y mostrar su UID y tipo por el monitor serial. En la función setup(), se configura la comunicación SPI con pines personalizados y se inicializa el lector. También se establece una clave por defecto que se usa para autenticar el acceso a un sector de la tarjeta.

Dentro del loop(), el programa verifica si hay una nueva tarjeta presente. Si la detecta y puede leer su número de serie (UID), lo muestra en formato hexadecimal junto con el tipo de tarjeta. Luego intenta autenticarse en el sector 0 utilizando la clave por defecto. Si la autenticación es exitosa, lee el bloque de memoria correspondiente y muestra las claves A y B. Finalmente, se detiene la comunicación con la tarjeta. Este proceso se repite continuamente mientras el lector esté encendido.
