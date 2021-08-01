#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <functions.hpp>
#include <Plotter.h>
#include <PID_AutoTune_v0.h>

#define ROTATION 8256

#define EXTERNAL_PLOTTER 0
#define ARDUINO_PLOTTER 0
#define SERIAL_PRINT 0

#if EXTERNAL_PLOTTER
Plotter p;
double Error;
#endif

PID_ATune aTune(&Input, &Output);

int E1 = 5;
int M1 = 4;
bool flag = true;
bool tuning = false;
double Setpoint, Input, Output;

PID myPID(&Input, &Output, &Setpoint, .2, 0.1, 0.01, DIRECT);
Encoder knobLeft(2, 3);

void setup()
{
  pinMode(21, INPUT); //UP KILL TRIGGER
  pinMode(20, INPUT); //DOWN KILL TRIGGER
  pinMode(19, INPUT); //RIGHT KILL TRIGGER
  pinMode(18, INPUT); //LEFT KILL TRIGGER

  Input = knobLeft.read();
  Setpoint = ROTATION;
  pinMode(M1, OUTPUT);

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);

  attachInterrupt(digitalPinToInterrupt(21), limitUp, CHANGE);
  attachInterrupt(digitalPinToInterrupt(20), limitDown, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), limitRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(18), limitLeft, CHANGE);

  if (SERIAL_PRINT || ARDUINO_PLOTTER)
  {
    Serial.begin(9600);
  }

#if EXTERNAL_PLOTTER
  p.Begin();

  // Add X-Y graphs
  // p.AddXYGraph("X-Y graph w/ 500 points", 500, "x axis", x, "y axis", y);
  // p.AddXYGraph("X-Y graph w/ 200 points", 200, "x axis", x, "y axis", y);

  // Add time graphs. Notice the effect of points displayed on the time scale
  p.AddTimeGraph("Time graph w/ 500 points", 500, "Error", Error);
  p.AddTimeGraph("Time graph w/ 200 points", 500, "Input", Input);
  p.AddTimeGraph("Time graph w/ 200 points", 500, "Output", Output);
#endif
}

void loop()
{
  if (flag) //RUN if kill switch not enabled
  {

    Input = knobLeft.read(); //READ current encoder value

    if (tuning) //IF tuning mode activated
    {
      byte val = (aTune.Runtime()); 
      if (val != 0)
      {
        tuning = false;
      }
      if (!tuning)
      { //we're done, set the tuning parameters
        int kp = aTune.GetKp();
        int ki = aTune.GetKi();
        int kd = aTune.GetKd();
        myPID.SetTunings(kp, ki, kd);
        Serial.print("Auto tune values Kp, Ki, Kd");
        Serial.println(kp);
        Serial.println(ki);
        Serial.println(kd);
      }
    }

    else
      myPID.Compute(); //Compute PID from library

    if (Output < 0) //if output, from -255 to 255 is -ve
    {
      digitalWrite(M1, LOW); //M1 Low  = reverse motor
    }
    else
    {
      digitalWrite(M1, HIGH); //M1 High = forward motor
    }

    analogWrite(E1, abs(Output)); //Output PWM of abs value of output

    if (SERIAL_PRINT)
    {
      Serial.print("Encoder = ");
      Serial.print(Input);
      Serial.print("  Error =  ");
      Serial.print(Setpoint - Input);
      Serial.print("  PWM Output = ");
      Serial.println(Output);
    }

    if (ARDUINO_PLOTTER)
    {
      Serial.print("Min:0,");
      Serial.print("Encoder");
      Serial.print(map(Input, -10000, 10000, 0, 100));
      Serial.print(",");
      Serial.print("Error");
      Serial.print(map((Setpoint - Input), -10000, 10000, 0, 100));
      Serial.print(",");
      Serial.print("PWM Output");
      Serial.print(map(Output, -255, 255, 200, 300));
      Serial.print(",");
      Serial.println("Max:300");
    }

  #if EXTERNAL_PLOTTER

    p.Plot();

  #endif
  }

  else
  {
    analogWrite(E1, 0);
    delay(99999);

    if (SERIAL_PRINT)
    {
      Serial.print("KILL\n");
    }
  }
}
