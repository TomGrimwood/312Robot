#include <Arduino.h>
/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(2, 3);
Encoder knobRight(7, 8);
//   avoid using pins with LEDs attached

void setup() {
      pinMode(M1, OUTPUT);
  Serial.begin(9600);
  Serial.println("TwoKnobs Encoder Test:");
  digitalWrite(M1,HIGH);
  analogWrite(E1, 50);
  pinMode(7, OUTPUT);

}

long positionLeft  = -999;
long positionRight = -999;

void loop() {
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobLeft.write(0);
    knobRight.write(0);
  }

  if (digitalRead(7)==HIGH)
  {
    //analogWrite(E1, 50);
    digitalWrite(M1,HIGH);
  }
  else
  {
    digitalWrite(M1,LOW);
  }
}