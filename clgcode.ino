#include <MFRC522.h> // RFID header file
#include <Keypad.h> // Keypad header file
#include <Servo.h> // Servo motor header file
#include <SPI.h> // Serial peripheral interface header file
#define SS_PIN 53
#define RST_PIN 5

Servo sg90;
# define servoPin 9
# define buzzerPin 8
MFRC522 mfrc522(SS_PIN, RST_PIN);
char initial_password[3] = {'8', '6', '8'};  // Variable to store initial password
String tagUID = "D9 75 FB C1"; // String to store UID of tag. Change it with your tag's UID
char password [3]; // Variable to store users’ password
boolean RFIDMode = true; // boolean to change modes
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0; 

const byte rows = 4;
const byte columns = 3;

char hexaKeys[rows][columns] = 
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;


byte row_pins[rows] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte column_pins[columns] = {12, 6, 7}; // connect to the column of the keypad

Keypad keypad_key = Keypad(makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
void setup () {
  // Arduino Pin configuration
 pinMode(buzzerPin, OUTPUT);
  sg90.attach(servoPin); //Declare pin 10 for servo
  sg90.write(0); // Set initial position at 90 degrees
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init (); // Init MFRC522
 
  
}

void loop () {
  // System will first look for mode
  if (RFIDMode == true) {
    
    if (! mfrc522.PICC_IsNewCardPresent ()) {
      return;
    }
    // Select one of the cards
    if (! mfrc522.PICC_ReadCardSerial ()) {
      return;
    }

    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
      Serial.println(tag);
   
 }
    tag.toUpperCase();
    //Checking the card
    if (tag.substring(1) == tagUID)
    {
      // If UID of tag is matched.
     
      digitalWrite(buzzerPin, HIGH);
      delay (200);
      digitalWrite(buzzerPin, LOW);
      
      
      RFIDMode = false; // Make RFID mode false
    }

    else
    {
      // If UID of tag is not matched.
      
      digitalWrite(buzzerPin, HIGH);
      
      delay (3000);
    digitalWrite(buzzerPin, LOW);
      
    }
  }

   if (RFIDMode == false) {
    key_pressed = keypad_key.getKey(); // Storing keys
    if (key_pressed)
    {
      password[i++] = key_pressed; // Storing in password variable
    
    }
    if (i == 3) // If 3 keys are completed
    {
      delay (200);
      if (! (strncmp(password, initial_password, 3))) // If password is matched
      {
        //digitalWrite(buzzerPin, LOW);
        sg90.write(90); // Door Opened
        
        delay (3000);
        
        sg90.write(0); // Door Closed
        
        i = 0;
        RFIDMode = true; // Make RFID mode true
      }

      else    // If password is not matched
      {
        
       digitalWrite(buzzerPin, HIGH);
        
        delay (3000);
        digitalWrite(buzzerPin, LOW);
        i = 0;
        RFIDMode = true; // Make RFID mode true
      }
    }
  }
}
