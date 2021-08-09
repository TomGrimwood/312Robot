#define E1 5
#define M1 4
#define E2 6
#define M2 7

#define LEFTMOTOR 1
#define RIGHTMOTOR 2

int limitFlag;
int input;

void setup() {
  Serial.begin(9600);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  
  pinMode(22,INPUT);
  pinMode(24,INPUT);
  pinMode(26,INPUT);
  pinMode(28,INPUT);

}

void loop() {



  
  if (Serial.available())  {
    input = Serial.read();
    
    if (digitalRead(22) || digitalRead(24) || digitalRead(26) || digitalRead(28)) {
      Serial.println("LIMIT SWITCH TRIPPED");
      input = 'x';
     
    }
    
    switch (input) {
    
      case 'w': 
        Serial.println("Moving Up");
        digitalWrite(M1, LOW);
        analogWrite(E1, 255);
        digitalWrite(M2, HIGH);
        analogWrite(E2, 255);   
        break;
        
      case 'a': 
        Serial.println("Moving Left");
        digitalWrite(M1, LOW);
        analogWrite(E1, 255);
        digitalWrite(M2, LOW);
        analogWrite(E2, 255);
        break;
        
      case 's': 
        Serial.println("Moving Down");
        digitalWrite(M1, HIGH);
        analogWrite(E1, 255);
        digitalWrite(M2, LOW);
        analogWrite(E2, 255);
        break;
        
      case 'd': 
        Serial.println("Moving Right");
        digitalWrite(M1, HIGH);
        analogWrite(E1, 255);
        digitalWrite(M2, HIGH);
        analogWrite(E2, 255);
        break;
        
      case 'x': 
        analogWrite(E1, 0);
        analogWrite(E2, 0);
        break;
        
    }
  }

  delay(100);
}
