#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <functions.hpp>


#define ROTATION -8256

#define SERIAL_PRINT 0


double Setpoint, Input, Output;

int E1 = 5;
int M1 = 4; 
int E2 = 6;
int M2 = 7; 
bool flag = true;

PID myPID(&Input, &Output, &Setpoint, .2, 0.1, 0.01, DIRECT);
Encoder knobLeft(2, 3);

void setup()
{
  pinMode(21, INPUT); //UP KILL TRIGGER
  pinMode(20, INPUT); //DOWN KILL TRIGGER
  pinMode(19, INPUT); //RIGHT KILL TRIGGER
  pinMode(18, INPUT); //LEFT KILL TRIGGER

  Input = knobLeft.read();
  Setpoint = .5*ROTATION;
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);


  if (SERIAL_PRINT)
  {
    Serial.begin(9600);
  }

}
void loop()
{
  while (flag) //RUN if kill switch not enabled
  {

    Input = knobLeft.read(); //READ current encoder value


      myPID.Compute(); //Compute PID from library

      moveRight(Output);

    if (SERIAL_PRINT)
    {
      Serial.print("Encoder = ");
      Serial.print(Input);
      Serial.print("  Error =  ");
      Serial.print(Setpoint - Input);
      Serial.print("  PWM Output = ");
      Serial.println(Output);
    }

    flag = !digitalRead(21);// || !digitalRead(18)// || !digitalRead(19) || !digitalRead(18);
  }

  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(99999);

  if (SERIAL_PRINT)
  {
    Serial.print("KILL\n");
  }
}
