#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <functions.hpp>
#define CENTIMETRE 1852
#define SQUARE_SIZE 3
#define ROTATION 5000
#define SAMPLE_TIME 10 //minimum time till PID recalculates  the  output (MilliSeconds)

double Setpoint, Input, Output;

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;
bool flag = true;

PID myPID(&Input, &Output, &Setpoint, 3, 0, 0.1, DIRECT);
Encoder knobLeft(2, 3);

void setup()
{
  //Serial.begin(9600);
  pinMode(LEFT_KILL_PIN, INPUT);
  pinMode(RIGHT_KILL_PIN, INPUT);
  pinMode(UP_KILL_PIN, INPUT);
  pinMode(DOWN_KILL_PIN, INPUT);

  pinMode(LEFT_DIRECTION_PIN, OUTPUT);
  pinMode(RIGHT_DIRECTION_PIN, OUTPUT);

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-DUTY_CYCLE_LIMIT, DUTY_CYCLE_LIMIT);
  myPID.SetSampleTime(SAMPLE_TIME);
  myPID.SetControllerDirection(1);
}
void loop()
{

  moveDistance(SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(-SQUARE_SIZE * CENTIMETRE, VERTICAL);
  moveDistance(-SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(SQUARE_SIZE * CENTIMETRE, VERTICAL);
  delay(2000);

  moveDistance(2 * SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(-2 * SQUARE_SIZE * CENTIMETRE, VERTICAL);
  moveDistance(-2 * SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(2 * SQUARE_SIZE * CENTIMETRE, VERTICAL);
  delay(2000);
  moveDistance(2.5 * SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(2.5 * -SQUARE_SIZE * CENTIMETRE, VERTICAL);
  moveDistance(2.5 * -SQUARE_SIZE * CENTIMETRE, HORIZONTAL);
  moveDistance(2.5 * SQUARE_SIZE * CENTIMETRE, VERTICAL);
  delay(2000);
}
