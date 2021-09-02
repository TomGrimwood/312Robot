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

void moveDistance(double distance, bool isVertical) //move set distance (+ve or -ve) in direction (vertical/horizontal)
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

      if (millis() > now + 45) //If 300ms has passed since last sample check
      {
        now = millis(); //update last sample check time to now

        if (Input == encoderPrevious && abs(Setpoint - Input) < ENCODER_CLOSE_ENOUGH) //if The encoder reading has not changed in the last 300ms (STALLED), and the error is within 50 encoder readings of desired.
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

void centre() //go to the middle(ish) of the board
{

  moveDistance(-32000, HORIZONTAL); //far left
  moveDistance(-32000, VERTICAL); //far bottom
  //GO bottom left
  moveDistance(18600, 0); //middle across
  moveDistance(12000, 1); //middle up

}

void moveMotors() //used for circle drawing
{

  myPID.Compute(); //calculates PID output from current positon, desired position etc.
  myOtherPID.Compute(); //calcultaes for both axis

  digitalWrite(RIGHT_DIRECTION_PIN, OutputR > 0); //sets direction depending if output is + or -

  digitalWrite(LEFT_DIRECTION_PIN, Output < 0);

  if (checkForAllMoves()) //if no kill switches are triggered
  {
    analogWrite(LEFT_PWM_PIN, abs(Output)); //outputs PWM duty cycle to pins
    analogWrite(RIGHT_PWM_PIN, abs(OutputR));
  }

  else // kill switches are triggered
  {
    analogWrite(LEFT_PWM_PIN, 0);     //kill motors
    analogWrite(RIGHT_PWM_PIN, 0);
  }
}
