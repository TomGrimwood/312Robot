#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#define ROTATION 8256
int E1 = 5;
int M1 = 4;
bool flag = true;
Encoder knobLeft(2, 3);
double Setpoint, Input, Output;
void limitUp();
void limitDown();
void limitLeft();
void limitRight();
PID myPID(&Input, &Output, &Setpoint, .2, 0.1, 0.01, DIRECT);

void setup()
{
  pinMode(21, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  Input = knobLeft.read();
  Setpoint = ROTATION;
  pinMode(M1, OUTPUT);
  Serial.begin(9600);
  //digitalWrite(M1, HIGH);
  //analogWrite(E1, 255);
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-100, 100);
  attachInterrupt(digitalPinToInterrupt(21), limitUp, CHANGE);
  attachInterrupt(digitalPinToInterrupt(20), limitDown, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), limitRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(18), limitLeft, CHANGE);
}

long positionLeft = -999;

void loop()
{
  while (flag)
  {

    Input = knobLeft.read();
    myPID.Compute();

    //     if (Input != positionLeft) {
    // Serial.print("Encoder = ");
    // Serial.print(Input);
    // Serial.print("     PWM Speed = ");
    // Serial.print(Output);

    // positionLeft = Input;
    //     }
    //         Serial.print("     PWM Speed = ");
    // Serial.print(Output);
    // Serial.print("  Motor State = ");
    if (Output < 0)
    {
      digitalWrite(M1, LOW);
      // Serial.print(" Reverse ");
    }
    else
    {
      // Serial.print("  Forward ");
      digitalWrite(M1, HIGH);
    }

    int OutputAbsolute = abs(Output);
    // Serial.print("  PWM OUTPUT = ");
    //   Serial.println(Output);
    //   Serial.print("\n\n\n\n");
    analogWrite(E1, OutputAbsolute);

    Serial.print("Encoder = ");
    Serial.print(knobLeft.read());
    Serial.print("    error =  ");
    Serial.print(Setpoint - knobLeft.read());
    Serial.print("     output = ");
    Serial.println(myPID.GetKp());
  }

  analogWrite(E1, 0);
  Serial.print("KILL\n");
  delay(99999);
}
void limitUp()
{
Serial.println("up hit");
Serial.println("FLAG SET TO FALSE");
  flag = false;
  delay(50);
}
void limitLeft()
{
Serial.println("left hit");
Serial.println("FLAG SET TO FALSE");
  flag = false;
    delay(50);
}
void limitDown()
{
Serial.println("down hit");
Serial.println("FLAG SET TO FALSE");
  flag = false;
    delay(50);
}
void limitRight()
{

  Serial.println("right hit");
  Serial.println("FLAG SET TO FALSE");
  flag = false;
    delay(50);
}
