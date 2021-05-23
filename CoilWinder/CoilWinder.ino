
const int stepX = 2;
const int dirX  = 5;
const int stepY = 3;
const int dirY  = 6;
const int enPin = 8;


#define M_ROTATE    1
#define M_SLIDE     0
#define FWD         LOW
#define BACK        HIGH

void runMotor(int motorId, int stepCount, int direction){
  int motor;

  if(motorId == 0){
    motor = stepX;
    digitalWrite(dirX, direction);
  }else{
    motor = stepY;
    digitalWrite(dirY, direction);
  }

  digitalWrite(enPin,LOW);
  for(int x = 0; x < stepCount; x++) {
    digitalWrite(motor,HIGH);
    delayMicroseconds(1000);
    digitalWrite(motor,LOW);
    delayMicroseconds(1000);
  }
  digitalWrite(enPin,HIGH);
  
}

void setup() {
  pinMode(stepX,OUTPUT);
  pinMode(dirX,OUTPUT);

  pinMode(stepY,OUTPUT);
  pinMode(dirY,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH);
}

void loop() {

  
 
  runMotor(M_ROTATE, 200, FWD);
  delay(1000); 
  runMotor(M_ROTATE, 200, BACK);
  delay(1000);   
  
  runMotor(M_SLIDE, 2000, FWD);
  delay(100); 
  runMotor(M_SLIDE, 2000, FWD);
  delay(100); 


}
