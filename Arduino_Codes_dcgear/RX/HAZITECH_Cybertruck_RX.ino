#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 5
#define in1 6
#define in2 7
#define in3 4
#define in4 2
#define enB 3
#define MOTOR_PIN 4  // DC motor vezérlése a D4-es lábon

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int CH1, CH2;
int motorSpeedA = 0;
int motorSpeedB = 0;
int channel[2];
bool txConnected = false;

void setup()
{
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);  // DC motor vezérlése a D4-es lábon

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

    txConnected = true;

    Serial.println(CH2);
    Serial.println(CH1);
  }
  else
  {
    txConnected = false;

    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  if (txConnected)
  {
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

    // Kormányzás DC motorral (D4-es láb)
    int motorSpeedC = map(CH1, 0, 1023, -255, 255); // Teljes tartomány a negatív és pozitív irányban

    if (motorSpeedC < 0)
    {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
    else if (motorSpeedC > 0)
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    else
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }

    analogWrite(MOTOR_PIN, abs(motorSpeedC)); // Abszolút érték a motor sebességéhez

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

  delay(100);
}
