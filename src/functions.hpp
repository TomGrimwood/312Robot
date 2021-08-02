#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <PID_v1.h>
extern bool flag;
extern int E1;
extern int M1;
extern int E2;
extern int M2;
extern PID myPID;
void limitUp();
void limitDown();
void limitLeft();
void limitRight();
void moveUp(int power);
void moveDown(int power);
void moveLeft(int power);
void moveRight(int power);


#endif