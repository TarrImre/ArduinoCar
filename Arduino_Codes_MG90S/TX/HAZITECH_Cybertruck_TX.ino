// HAZI TECH
// Programmed by Hasitha Jayasundara
// Visit my YouTube Channel for more info - http://www.youtube.com/c/HAZITECH?sub_confirmation=1
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char xyData[32] = "";
int channel[2];

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
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

  delay(200); // Várakozás 1 másodpercig
}
