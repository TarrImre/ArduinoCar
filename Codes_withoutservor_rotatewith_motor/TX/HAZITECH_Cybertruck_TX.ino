#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char xyData[32] = "";
int channel[2];
const int redLedPin = 7;
const int greenLedPin = 6;
bool isConnected = false;

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  digitalWrite(redLedPin, LOW);
  // Piros LED bekapcsolása
  digitalWrite(redLedPin, HIGH);
}

void loop() 
{
  channel[0] = analogRead(A0);
  channel[1] = analogRead(A1);
  
  radio.write(channel, sizeof(channel));

  // Kiírás a soros monitorra
  Serial.print("Channel 0: ");
  Serial.print(channel[0]);
  Serial.print("\tChannel 1: ");
  Serial.println(channel[1]);

  // Kapcsolat ellenőrzése
  isConnected = radio.isChipConnected();
  // Zöld LED be- vagy kikapcsolása attól függően, hogy van-e kapcsolat
  if (isConnected) {
    digitalWrite(greenLedPin, HIGH); // Van kapcsolat
  } else {
    digitalWrite(greenLedPin, LOW); // Nincs kapcsolat
  }
}
