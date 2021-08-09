#include <Encoder.h>

#define E1 5
#define M1 4
#define E2 6
#define M2 7

#define LEFTMOTOR 1
#define RIGHTMOTOR 2

#define PULLEYDIAMETER 64
#define COUNTPERREV 8256
#define BIAS12V 30
#define BIAS5V 50

#define MATHPI 3.1416

Encoder knobLeft(2, 3);
Encoder knobRight(4, 5);
float distanceTravelledTest;

void setup() {

  Serial.begin(9600);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

}

void loop() {

  delay(1000);

//  distance_control(LEFTMOTOR, -2 * MATHPI * PULLEYDIAMETER);
  distance_control(RIGHTMOTOR, 2 * MATHPI * PULLEYDIAMETER);


//  motor_power(LEFTMOTOR, 255);
//  Serial.println(knobLeft.read());
//  distanceTravelledTest = count_to_mm(knobLeft.read());
//  Serial.println(distanceTravelledTest);
  
//  distance_control(5 * MATHPI * PULLEYDIAMETER);
//  delay(1000);
//  distance_control(-1 * MATHPI * PULLEYDIAMETER);
//  delay(1000);
//  distance_control(3 * MATHPI * PULLEYDIAMETER);  
//  delay(1000);
//  distance_control(-3 * MATHPI * PULLEYDIAMETER);  
//  delay(1000);
}

void motor_power(int motorSelected, int power) {

  int outputPower = (int) saturate(abs(power) + BIAS12V, 0, 255);
  
  switch (motorSelected) {
    case 1:

      digitalWrite(M1, power >= 0);
      analogWrite(E1, outputPower);
      break;

    case 2:

      digitalWrite(M2, power >= 0);
      analogWrite(E2, outputPower);
      break;

  }
}

void distance_control(int motorSelected, int distanceToTravel) {

  int power, limit = 255;
  float errorDistance, distanceTravelled, lastErrorDistance = 0;
  float Kp = 2, Ki = 0.01, Kd = 15;
  float proportional, integral = 0, derivative;
  int period = 10, stallCheck = 0, stallTime = 100;

  if (motorSelected == LEFTMOTOR) {
    knobLeft.write(0);
  } else if (motorSelected == RIGHTMOTOR) {
    knobRight.write(0);
  }

  do {

    if (motorSelected == LEFTMOTOR) {
      distanceTravelled = count_to_mm(knobLeft.read());
    } else if (motorSelected == RIGHTMOTOR) {
      distanceTravelled = count_to_mm(knobRight.read());
    }
    
    errorDistance = distanceToTravel - distanceTravelled;
   
    proportional = Kp * errorDistance;
    
    if (lastErrorDistance >= 0) {
      integral = integral * (errorDistance >= 0);
    } else {
      integral = integral * (errorDistance < 0);
    }
    
    integral += Ki*errorDistance;
    integral = integral * !((proportional > 100) || (proportional < -100));
    
    derivative = Kd * (errorDistance - lastErrorDistance);

    power = proportional + integral + derivative;


    

    Serial.print(errorDistance);
    Serial.print("\t");
    Serial.print(saturate(proportional, -255, 255));
    Serial.print("\t");
    Serial.print(integral);
    Serial.print("\t");
    Serial.print(derivative);
    Serial.print("\t");
    Serial.println(saturate(power, -255, 255));


    power = saturate(power, -limit, limit);

    motor_power(motorSelected, power);

    errorDistance = round_2dp(errorDistance);

    if ((errorDistance == lastErrorDistance) || (abs(errorDistance) < 0.8)) {
      stallCheck++;
    } else {
      stallCheck = 0;
    }
    lastErrorDistance = errorDistance;

    delay(period);

  } while (stallCheck * period < stallTime);

  motor_power(motorSelected, 0);
  Serial.println("********** FINAL ERROR (mm) **********");
  Serial.println(errorDistance);
  Serial.println("**************************************");
}

//Converts wheel encoder counts to mm
float count_to_mm(long wheelEncoderCount) {
  float mmDistance;
  mmDistance = (wheelEncoderCount * MATHPI * PULLEYDIAMETER) / COUNTPERREV;
  return mmDistance;
}

//int mm_to_count(long distance) {
//  int count = ((float)distance * COUNTPERREV) / (MATHPI * PULLEYDIAMETER);
//  return count;
//}

int saturate(int input, int lower, int upper) {

  if (input >= upper) {
    return upper;
  } else if (input <= lower) {
    return lower;
  } else {
    return input;
  }

}

float round_2dp(float number) {

  return number = (float)round(number * 100) / 100;

}
