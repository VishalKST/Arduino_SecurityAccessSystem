#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Slave Select Pin
#define RST_PIN 9  // Reset Pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card

  Serial.println("RFID Reader Initialized");
  Serial.println("Ready to scan");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }

  // Show UID on serial monitor
  Serial.print("Tag UID:");
  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    tagUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(tagUID);
  mfrc522.PICC_HaltA();  // Stop reading
}
