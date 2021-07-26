#include <Arduino.h>

// #include <Encoder.h>
// int E1 = 5;
// int M1 = 4;
// int E2 = 6;
// int M2 = 7;
// Encoder knobLeft(2, 3);
// Encoder knobRight(7, 8);

// #define LEFT 21
// #define RIGHT 20
// #define UP 19
// #define DOWN 18


// void setup()
// {
//   pinMode(M1, OUTPUT);
//   Serial.begin(9600);
//   Serial.println("TwoKnobs Encoder Test:");
//   digitalWrite(M1, HIGH);
//   analogWrite(E1, 50);
//   pinMode(7, OUTPUT);
//   attachInterrupt(digitalPinToInterrupt(18), test, RISING);
// }

// long positionLeft = -999;
// long positionRight = -999;

// void loop()
// {
//   long newLeft, newRight;
//   newLeft = knobLeft.read();
//   newRight = knobRight.read();
//   // if (newLeft != positionLeft || newRight != positionRight)
//   // {
//   //   Serial.print("Left = ");
//   //   Serial.print(newLeft);
//   //   Serial.print(", Right = ");
//   //   Serial.print(newRight);
//   //   Serial.println();
//   //   positionLeft = newLeft;
//   //   positionRight = newRight;
//   // }
//   // if a character is sent from the serial monitor,
//   // reset both back to zero.
//   if (Serial.available())
//   {
//     Serial.read();
//     Serial.println("Reset both knobs to zero");
//     knobLeft.write(0);
//     knobRight.write(0);
//   }

//   if (digitalRead(7) == HIGH)
//   {
//     //analogWrite(E1, 50);
//     digitalWrite(M1, HIGH);
//   }
//   else
//   {
//     digitalWrite(M1, LOW);
//   }
// }


// void test() {
//   Serial.println("INTERRUPT LEFT");
// }

void blink();
const byte ledPin = 13;
const byte interruptPin = 18;
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  Serial.begin(9600);
  Serial.println("hello");
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  Serial.println("INTERRUPT LEFT");
}
