#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 5
#define in1 6
#define in2 7
#define in3 4
#define in4 2
#define enB 3
#define SERVO_PIN 10
#define redLED A0
#define whiteLED A1

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
  pinMode(redLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(whiteLED, HIGH);
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

    Serial.print("CH1: ");
    Serial.print(CH1);
    Serial.print("\tCH2: ");
    Serial.println(CH2);
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
    if (CH2 < 470)
    {
      if (CH1 < 470)
      {
          forwardLeft();  
        motorSpeedA = map(CH2, 470, 0, 0, 255);
        motorSpeedB = map(CH2, 470, 0, 0, 255);
      }
      else if (CH1 > 550)
      {
        forwardRight();
        motorSpeedA = map(CH2, 470, 0, 0, 255);
        motorSpeedB = map(CH2, 470, 0, 0, 255);
      }
      else
      {
        forward();
        motorSpeedA = map(CH2, 470, 0, 0, 255);
        motorSpeedB = map(CH2, 470, 0, 0, 255);
      }
    }
    else if (CH2 > 550)
    {
      if (CH1 < 470)
      {
        backwardLeft();
        motorSpeedA = map(CH2, 550, 1023, 0, 255);
        motorSpeedB = map(CH2, 550, 1023, 0, 255);
      }
      else if (CH1 > 550)
      {
        backwardRight();
        motorSpeedA = map(CH2, 550, 1023, 0, 255);
        motorSpeedB = map(CH2, 550, 1023, 0, 255);
      }
      else
      {
        backward();
        motorSpeedA = map(CH2, 550, 1023, 0, 255);
        motorSpeedB = map(CH2, 550, 1023, 0, 255);
      }
    }
    else if (CH1 < 470)
    {
      left();
      motorSpeedA = map(CH1, 470, 0, 0, 255);
      motorSpeedB = map(CH1, 470, 0, 0, 255);
    }
    else if (CH1 > 550)
    {
      right();
      motorSpeedA = map(CH1, 550, 1023, 0, 255);
      motorSpeedB = map(CH1, 550, 1023, 0, 255);
    }
    else
    {
      motorSpeedA = 0;
      motorSpeedB = 0;
      digitalWrite(redLED, LOW);
    }

    analogWrite(enA, motorSpeedA);
    analogWrite(enB, motorSpeedB);
  }
}
void backward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  backwardLamp(); // Hátrafelé mozgásnál világít a lámpa
}

void backwardLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  backwardLamp(); // Hátrafelé mozgásnál világít a lámpa
}

void backwardRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  backwardLamp(); // Hátrafelé mozgásnál világít a lámpa
}

void forward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(redLED, LOW); // Kikapcsolja a hátlámpát
}

void forwardLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(redLED, LOW); // Kikapcsolja a hátlámpát
}

void forwardRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(redLED, LOW); // Kikapcsolja a hátlámpát
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(redLED, LOW); // Kikapcsolja a hátlámpát
}

void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(redLED, LOW); // Kikapcsolja a hátlámpát
}

void backwardLamp() {
  digitalWrite(redLED, HIGH);
}
