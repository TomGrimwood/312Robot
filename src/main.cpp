#include <Arduino.h>
#include <Encoder.h>
#include <PID_v1.h>
#include <functions.hpp>

#define CENTIMETRE 1852 //encoder count for 1cm
#define SQUARE_SIZE 3
#define CIRCLE_SIZE 3 //in centimetres
#define ROTATION 5000
#define SAMPLE_TIME 1 //minimum time till PID recalculates  the  output (MilliSeconds)
#define RESOLUTION 100 //amount of pos recalculations of circle;
#define TRUE_FOR_CIRCLE 1;

double Setpoint, Input, Output;
double SetpointR, InputR, OutputR;

int E1 = LEFT_PWM_PIN;
int M1 = LEFT_DIRECTION_PIN;
int E2 = RIGHT_PWM_PIN;
int M2 = RIGHT_DIRECTION_PIN;

PID myPID(&Input, &Output, &Setpoint, 3, 0, 0.1, DIRECT);
PID myOtherPID(&InputR, &OutputR, &SetpointR, 3, 0, 0.1, DIRECT);
Encoder knobLeft(2, 3);
Encoder knobRight(3, 4);

void setup()
{

  #if TRUE_FOR_CIRCLE == 1;
  // http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html
  // TIMER 1 for interrupt frequency 200 Hz:
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 200 Hz increments
OCR1A = 9999; // = 16000000 / (8 * 200) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 8 prescaler
TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interrupts
#endif


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

  myOtherPID.SetMode(AUTOMATIC);
  myOtherPID.SetOutputLimits(-DUTY_CYCLE_LIMIT, DUTY_CYCLE_LIMIT);
  myOtherPID.SetSampleTime(SAMPLE_TIME);
  myOtherPID.SetControllerDirection(1);
}
void loop()
{

  centre();

  //Move to far Right side to begin circle draw
  SetpointR = CIRCLE_SIZE * CENTIMETRE;

//wait for it to hopefully get there
  delay(4000);

  knobLeft.write(CIRCLE_SIZE * CENTIMETRE);
  knobRight.write(0);

  for (int i = 0; i < RESOLUTION; i++)
  {

    SetpointR  = CIRCLE_SIZE * CENTIMETRE * cos(2*PI*i/RESOLUTION);
    Setpoint   = CIRCLE_SIZE * CENTIMETRE * sin(2*PI*i/RESOLUTION);

    delay( 50000 / RESOLUTION);

  }
  


}

ISR(TIMER1_COMPA_vect){

   moveMotors();

}
