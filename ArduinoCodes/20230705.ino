#include <Servo.h>
//#include <SoftwareSerial.h>

Servo sT;Servo sR;Servo sL;
#define trigPin1 A0
#define echoPin1 A1
#define trigPin2 A2
#define echoPin2 A3

#define laser 2
#define lamp 4
#define buz 10

#define elvUp 8
#define elvDown 9
#define rottor 7

#define ledR 11
#define ledG 12
#define ledB 13

char inp;

long leftSensor,rightSensor, verticalLevel;

int landScannerState = 1;


void setup() {  

  Serial.begin(9600);
  //HC06.begin(9600);
  sT.attach(5);
  sL.attach(6);
  sR.attach(3);

  pinMode(echoPin1,INPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin1,OUTPUT);
  pinMode(trigPin2,OUTPUT);

  pinMode(lamp,OUTPUT);
  //pinMode(buz,OUTPUT);
  pinMode(elvUp,OUTPUT);
	pinMode(elvDown,OUTPUT);
  pinMode(rottor,OUTPUT);
  
  pinMode(laser,OUTPUT);

  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(ledB,OUTPUT);

  //change relay state to NC
  digitalWrite(elvUp, HIGH);  
  digitalWrite(rottor, HIGH);  
//rotateServo(90);
  /*Rotor(1);
  Elevator(1);
  digitalWrite(lamp, HIGH);  
  digitalWrite(laser, HIGH);  */
//Serial.println(getVerticalLevel(0));
  landScannerState = 1;
  digitalWrite(laser, HIGH); 
}
void inputLoop(){
  if(Serial.available()){
    inp = Serial.read();
  }

  if(inp=='1'){
    ElevatorUp();
  }
  else if(inp=='0'){
    ElevatorDown();
  }
  else if(inp=='2'){
    ElevatorUpAuto();
  }
  else if(inp=='3'){
    ElevatorOnDelay(1, 8000);
    delay(1000);
    
  }

  if(landScannerState!=0){
    landScanner();
  }
  
  
leftSensor = getUltrasonicData('L');
  rightSensor = getUltrasonicData('R');

  Serial.print(leftSensor);
  Serial.print("\t\t");
  Serial.println(rightSensor);
  delay(500);
}

void loop() {
  
landScanner();

}

void landScanner(){
  getVerticalLevel(0);
  int angle, rotateFrom=0, rotateTo=180, increment = 10,rotateTimeGap = 1000;
  long diagonal, errorRange = 5;
  if(landScannerState%2==0){
    rotateFrom=180, rotateTo=0;increment = -10;
  }
  if(landScannerState==1 || landScannerState==5){
    rotateServo(85);
    diagonal = verticalLevel / .9961;
  }
  else if(landScannerState==2 || landScannerState==6){
    rotateServo(80);
    diagonal = verticalLevel / .9848;
  }
  else if(landScannerState==3 || landScannerState==7){
    rotateServo(75);
    diagonal = verticalLevel / .9609;
  }
  else{
    rotateServo(70);
    diagonal = verticalLevel / .9396;
  }
  while((rotateFrom<=rotateTo && landScannerState%2!=0) || (rotateFrom>=rotateTo && landScannerState%2==0)){
    topServo(rotateFrom);
    delay(10);
    leftSensor = getUltrasonicData('L');
    rightSensor = getUltrasonicData('R');

    Serial.print(landScannerState);
  Serial.print('\t');
  Serial.print(rotateFrom);
  Serial.print('\t');
  Serial.print(leftSensor);
  Serial.print('\t');
  Serial.println(rightSensor);
  

   /* printLandScan('L', rotateFrom);
    printLandScan('R', rotateFrom); 
    if((leftSensor-errorRange)>verticalLevel && (leftSensor+errorRange)<verticalLevel ){
      Serial.println("G-" + rotateFrom + "-" + leftSensor);
    }*/
    rotateFrom+=increment;
    delay(rotateTimeGap);
  }
  landScannerState++;
  if(landScannerState==9){
    landScannerState = 1;
  }
}
void printLandScan(char c, int ang1){
  long d = leftSensor;
  if(c=='R'){
    d = rightSensor;
  }
  Serial.print(c);
  Serial.print('-');
  Serial.print(ang1);
  Serial.print('-');
  Serial.println(d);
}
void ElevatorUpAuto(){
  int ElevatorUpAutoCounter = 0;
  ElevatorOnDelay(1, 8000);
/*
  while(1){
    delay(10);
    long level = getVerticalLevel(0);

    if((level > 60)){
      Serial.println("level");break;
    }else if(ElevatorUpAutoCounter>15){
      Serial.println("count");break;
    }
    else{
      ElevatorOnDelay(1, 0); 
    }
    ElevatorUpAutoCounter++;
  }
*/
}

long getVerticalLevel(int t){
  sT.write(90);
  rotateServo(90);

  leftSensor = getUltrasonicData('L');
  rightSensor = getUltrasonicData('R');
/*
  Serial.print(leftSensor);
  Serial.print("\t\t");
  Serial.println(rightSensor);*/
  verticalLevel = (leftSensor+rightSensor)/2;
  return (leftSensor+rightSensor)/2;
}


void ElevatorUp(){
  ElevatorOnDelay(1, 0);
}

void ElevatorDown(){
  ElevatorOnDelay(0, 0);
 
}

void ElevatorOnDelay(int x, int workDelay){
  if(workDelay==0){
    workDelay = 700;
  }
  if(x==1){
    Elevator(1)   ;
    delay(workDelay);
    Elevator(0);
  }
  else{
    Elevator(2)   ;
    delay(workDelay);
    Elevator(0); 
    
  }
} 

void Elevator(int x){
  int eleDelay = 0;
  if(x==1){
    digitalWrite(elvDown, LOW);
    delay(eleDelay);
    digitalWrite(elvUp, LOW);    
  }else if(x==2){
    digitalWrite(elvUp, HIGH);
    delay(eleDelay);
    digitalWrite(elvDown, HIGH);
  }else{
    digitalWrite(elvDown, LOW);
    digitalWrite(elvUp, HIGH);  
  }
} 
void topServo(int a){
  sT.write(a + 0);
  
}

void rotateServo(int a){
  sR.write(a + 10);
  sL.write(a-5);
}

void rotateServoSameDirection(int a){
  sR.write(a + 10);
  sL.write(180 - a-5);
}

long getUltrasonicData(char PIN)
{
  int trig, echo;
  long duration;
  digitalWrite(laser, LOW);

  trig = trigPin2;
  echo = echoPin2;

  if(PIN=='L'){
    trig = trigPin1;
    echo = echoPin1;
  }

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  digitalWrite(laser, HIGH);

  return (duration/2) / 29.1;
  
}

void Rotor(int x){
  if(x==1){
    digitalWrite(rottor, LOW);    
  }else{
    digitalWrite(rottor, HIGH);
  }
}


void LED(char x){
  digitalWrite(ledR,LOW);
  digitalWrite(ledG,LOW);
  digitalWrite(ledB,LOW);

  if(x=='R'){
    digitalWrite(ledR,HIGH);
  }
  else if(x=='G'){
    digitalWrite(ledG,HIGH);
  }
  else if(x=='B'){
    digitalWrite(ledB,HIGH);
  }
}