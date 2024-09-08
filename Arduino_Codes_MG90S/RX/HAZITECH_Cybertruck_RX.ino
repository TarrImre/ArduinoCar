#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define enA 5
#define in1 6
#define in2 7
#define in3 4
#define in4 2
#define enB 3
#define SERVO_PIN 10

Servo myservo;
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int CH1, CH2;
int motorSpeedA = 0;
int motorSpeedB = 0;
int channel[2];
bool txConnected = false;  // Új változó a TX kapcsolat állapotának tárolására

void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  myservo.attach(10);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void loop()
{
  if (radio.available())
  {
    radio.read(channel, sizeof(channel));
    radio.read(&receivedData, sizeof(receivedData));
    CH2 = channel[0];
    CH1 = channel[1];

    // Ha a TX kapcsolatot érzékeli, akkor engedélyezi a vezérlőműveleteket
    txConnected = true;

    Serial.println(CH2);
    Serial.println(CH1);
  }
  else
  {
    // Ha nincs TX kapcsolat, letiltja a vezérlőműveleteket
    txConnected = false;

    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  // Csak akkor hajtja végre a vezérlőműveleteket, ha a TX kapcsolat aktív
  if (txConnected)
  {
    // A további vezérlőműveletek itt vannak

    if (CH2 < 470)
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      motorSpeedA = map(CH2, 470, 0, 0, 255);
      motorSpeedB = map(CH2, 470, 0, 0, 255);
    }
    else if (CH2 > 550)
    {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

      motorSpeedA = map(CH2, 550, 1023, 0, 255);
      motorSpeedB = map(CH2, 550, 1023, 0, 255);
    }
    else
    {
      motorSpeedA = 0;
      motorSpeedB = 0;
    }

    myservo.write((CH1 + 520) / 10);

    if (motorSpeedA < 70)
    {
      motorSpeedA = 0;
    }
    if (motorSpeedB < 70)
    {
      motorSpeedB = 0;
    }

    analogWrite(enA, motorSpeedA);
    analogWrite(enB, motorSpeedB);
  }

}
