#include "BluetoothSerial.h"
#include <MFRC522.h> //library responsible for communicating with the module RFID-RC522
#include <SPI.h> //library responsible for communicating of SPI bus


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define SS    21
#define RST   22
#define led   13

// Defined pins to module RC522
MFRC522 mfrc522(SS, RST); 


BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  SerialBT.begin("ESP32-miniProject"); //Bluetooth device name
  pinMode(led,OUTPUT);
  
  // Init MFRC522
  mfrc522.PCD_Init(); 
 
}

String Led = "Led mati";

void loop(){ 
  String code="";
  if ( mfrc522.PICC_IsNewCardPresent()){
    if ( mfrc522.PICC_ReadCardSerial()){
      for(byte i = 0; i < mfrc522.uid.size; i++) {
        String a =  mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
        String b = String ( mfrc522.uid.uidByte[i], HEX);
        String tampung = a+b;
        code = code+tampung;
      }

      Serial.print("ID:");
      Serial.println(code);
      
      SerialBT.print("ID:");
      SerialBT.println(code);
      mfrc522.PICC_HaltA();

      if(code == " ca 10 53 19" || "17 28 51 34 "){
           if(Led == "Led mati"){
          digitalWrite(led,HIGH);
          Led = "Lampu nyala";
          Serial.println(Led);
        }else{
          digitalWrite(led,LOW);
          Led = "Lampu mati";
          Serial.println(Led);
        }
      }  
       
    }
    }
}
