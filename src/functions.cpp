#include <functions.hpp>

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

void moveUp(int power) //where power  can be -255 to  255 where 255 = up, -255 = down
{
  myPID.SetControllerDirection(0);
  if (power > 0 )
  {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
  }

  else
  {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  }

  analogWrite(E1, abs(power));
  analogWrite(E2, abs(power));
}

void moveDown(int power)
{
  myPID.SetControllerDirection(1);
  if (power > 0 )
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
  }

  else
  {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  }

  analogWrite(E1, abs(power));
  analogWrite(E2, abs(power));
}

void moveLeft(int power)
{
  myPID.SetControllerDirection(1);
  if (power > 0 )
  {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
  }

  else
  {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  }

  analogWrite(E1, abs(power));
  analogWrite(E2, abs(power));
}

void moveRight(int power)
{
  myPID.SetControllerDirection(0);
  if (power > 0 )
  {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
  }

  else
  {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  }

  analogWrite(E1, abs(power));
  analogWrite(E2, abs(power));
}