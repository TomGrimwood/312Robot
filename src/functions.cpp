#include <functions.hpp>

void powerMotorsVertical() //Powers the motors vertically given a specific PWM Output (-255 - 255)
{

  if (Output > 0)
  {
    digitalWrite(LEFT_DIRECTION_PIN, LOW);
    digitalWrite(RIGHT_DIRECTION_PIN, HIGH);
  }

  else
  {
    digitalWrite(LEFT_DIRECTION_PIN, HIGH);
    digitalWrite(RIGHT_DIRECTION_PIN, LOW);
  }

  analogWrite(LEFT_PWM_PIN, abs(Output));
  analogWrite(RIGHT_PWM_PIN, abs(Output));
}

void powerMotorsHorizontal() //Powers the motors Horizontally given a specific PWM Output (-255 - 255)
{
  if (Output > 0)
  {
    digitalWrite(LEFT_DIRECTION_PIN, BACK);
    digitalWrite(RIGHT_DIRECTION_PIN, BACK);
  }

  else
  {
    digitalWrite(LEFT_DIRECTION_PIN, FORWARD);
    digitalWrite(RIGHT_DIRECTION_PIN, FORWARD);
  }

  analogWrite(LEFT_PWM_PIN, abs(Output));
  analogWrite(RIGHT_PWM_PIN, abs(Output));
}

bool checkForMove(bool axis, bool direction) //Returns true if a given move command is able to be executed safely, Axis being Vertical or Horizontal TRUE,FALSE  respectivley , and direction FALSE -ve if Left or Down
{
  //converts 2 bools into 2 bit number (0-3)
  //UP = 3 , LEFT = 0 , DOWN = 2 , RIGHT = 1
  uint8_t value = (axis ? 2 : 0) + (direction ? 1 : 0); //converts 2 bools into 2 bit number (0-3) representing the 4 possible directions

  return !digitalRead(value * 2 + 22); //if Pins 22 , 24, 26, 28 = LEFT RIGHT DOWN UP, it will poll the correct pin depending on intended direciton of motion
}

bool checkForAllMoves() //checks if any kill switches are triggered.
{
  return (!digitalRead(UP_KILL_PIN) &&
          !digitalRead(LEFT_KILL_PIN) &&
          !digitalRead(RIGHT_KILL_PIN) &&
          !digitalRead(DOWN_KILL_PIN));
}

void moveDistance(double distance, bool isVertical)
{

  unsigned long now = millis(); //Get current time at function start

  knobLeft.write(0); //Set encoder to 0

  isVertical ? Setpoint = distance : Setpoint = -distance; //If vertical, setpoint = distance, otherwise = -ve distance (dont ask why)

  int encoderPrevious = knobLeft.read(); // Previous encoder reading = current encoder reading for initialization

  while (checkForMove(isVertical, distance > 0)) //while it is safe to move in specified direction
  {
    Input = knobLeft.read(); //get input (current encoder count), from encoder library

    if (myPID.Compute()) //Compute required output PWM (-255 - 255 range), Will return false if SAMPLE_TIMES duration has not been exceed (ms)
    {
      isVertical ? powerMotorsVertical() : powerMotorsHorizontal(); //If BOOLEAN is vertical is true, powerMotorsVertically, otherwise...

      if (millis() > now + 300) //If 300ms has passed since last sample check
      {
        now = millis(); //update last sample check time to now

        if (Input == encoderPrevious && abs(Setpoint - Input) < 50) //if The encoder reading has not changed in the last 300ms (STALLED), and the error is within 50 encoder readings of desired.
        {
          //Serial.print("Not ");
          break; //exit the while loop and finish the moveDistance function call
        }
        else
        {
          encoderPrevious = Input; //if End conditions are not satisfied, update previous encoder reading to = current encoder reading
        }
      }
    }
  }

  //Serial.println("Quit due to kill switch");
  //Serial.println(Setpoint - Input);
  analogWrite(LEFT_PWM_PIN, 0); //KILL motors as precaution when exiting function
  analogWrite(RIGHT_PWM_PIN, 0);
}

void centre()
{

  moveDistance(-32000, HORIZONTAL); //
  moveDistance(-30000, VERTICAL);
  //GO bottom left
  moveDistance(18600, 0); //
  moveDistance(12000, 1);
  //go to middle ( need to be fine tuned. )
  knobLeft.write(0);
  knobRight.write(0);
}

void moveMotors()
{

  myPID.Compute();
  myOtherPID.Compute();

  if (OutputR > 0)
  {
    digitalWrite(RIGHT_DIRECTION_PIN, HIGH);
  }
  else
  {
    digitalWrite(RIGHT_DIRECTION_PIN, LOW);
  }

  if (Output > 0)
  {
    digitalWrite(LEFT_DIRECTION_PIN, HIGH);
  }
  else
  {
    digitalWrite(LEFT_DIRECTION_PIN, LOW);
  }

  if (checkForAllMoves())
  {
    analogWrite(LEFT_PWM_PIN, abs(Output));
    analogWrite(RIGHT_PWM_PIN, abs(OutputR));
  }

  else
  {
     analogWrite(LEFT_PWM_PIN, 0);
     analogWrite(RIGHT_PWM_PIN, 0);
  }
}
