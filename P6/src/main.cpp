//Álvaro Marcos Rodríguez


//1

/*#include <SPI.h>
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
}*/


// 2

/*
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
}*/