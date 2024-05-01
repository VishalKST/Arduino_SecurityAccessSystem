#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Define the SS_PIN
#define RST_PIN 9  // Define the RST_PIN

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

#define RED_LED 2      // Pin for red LED
#define GREEN_LED 3    // Pin for green LED
#define BUZZER 4       // Pin for buzzer

void setup() {
  Serial.begin(9600);       // Initialize serial communication
  SPI.begin();              // Init SPI bus
  mfrc522.PCD_Init();       // Init MFRC522

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  digitalWrite(RED_LED, HIGH);  // Start with red LED on
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Card detected, try to read UID
    String tag = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      tag.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    tag.toUpperCase();  // Convert to uppercase

    // Check if the tag is in the database
    if (checkDatabase(tag)) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      Serial.println("Accepted");
      digitalWrite(BUZZER, HIGH);  // Turn on buzzer
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(100);
      digitalWrite(BUZZER, HIGH);  // Turn on buzzer
      delay(200);
      digitalWrite(BUZZER, LOW);  // Turn off buzzer
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Serial.println("Access denied");
      digitalWrite(BUZZER, HIGH);  // Turn on buzzer
      delay(2000);
      digitalWrite(BUZZER, LOW);   // Turn off buzzer
    }

    mfrc522.PICC_HaltA();  // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
  }
}

bool checkDatabase(String tag) {
  // Compare the scanned tag with the valid tag IDs
  String validTag1 = "D36B741E";  // First valid tag
  String validTag2 = "498F945D";  // Second valid tag
  return tag.equals(validTag1) || tag.equals(validTag2);
}
