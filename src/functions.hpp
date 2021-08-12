#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <PID_v1.h>
#include <Encoder.h>

extern double Setpoint;
extern double Output;
extern double Input;

extern double SetpointR;
extern double OutputR;
extern double InputR;

extern PID myPID, myOtherPID;
extern Encoder knobLeft, knobRight;

#define RIGHT_KILL_PIN 24
#define LEFT_KILL_PIN 22
#define UP_KILL_PIN 28
#define DOWN_KILL_PIN 26

#define LEFT_DIRECTION_PIN 4
#define LEFT_PWM_PIN 5
#define RIGHT_DIRECTION_PIN 7
#define RIGHT_PWM_PIN 6

#define VERTICAL 1
#define HORIZONTAL 0

#define LEFT 0
#define RIGHT 1
#define UP 3
#define DOWN 2

#define FORWARD 1
#define BACK 0
#define DUTY_CYCLE_LIMIT 255


void moveDistance(double distance, bool isVertical);
void powerMotorsVertical();
void powerMotorsHorizontal();
bool checkForMove(bool axis, bool direction);
void centre();
void moveMotors();

#endif