#include <SPI.h> // Import Serial Peripheral Interface library for RC522
#include <MFRC522.h> // Import library for RC522 RFID Module
#include <LiquidCrystal.h> // Import library for LCD Display

#define RST_PIN 5 //
#define SS_PIN 53 //
#define READ_PIN 2 //
#define WRITE_PIN 3 //

MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

byte NEW_UID[4];

void setup() {
  Serial.begin(9600);
  SPI.begin();
  lcd.begin(16, 2);
  mfrc522.PCD_Init();  // Init MFRC522 card
  pinMode(READ_PIN, INPUT_PULLUP);
  pinMode(WRITE_PIN, INPUT_PULLUP);
}

void loop() {
  lcd.clear();
  lcd.print("Standby");

  // Wait until new card is present
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }

  // Read function
  if (digitalRead(READ_PIN) == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("READING...");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("UID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      lcd.print(mfrc522.uid.uidByte[i], HEX);
      NEW_UID[i] = (mfrc522.uid.uidByte[i]);
    }
    lcd.setCursor(0, 0);
    delay(1000);
  }

  //Write function
  if (digitalRead(WRITE_PIN) == LOW) {
    lcd.clear();
    lcd.print("WRITING");
    delay(500);
    lcd.setCursor(0, 0);
    if ( mfrc522.MIFARE_SetUid(NEW_UID, (byte)4, true) ) {
      lcd.print("Written!");
    }
    lcd.setCursor(0, 1);
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      lcd.print(NEW_UID[i], HEX);
    }
    delay(1000);
  }
  lcd.setCursor(0, 0);
}
