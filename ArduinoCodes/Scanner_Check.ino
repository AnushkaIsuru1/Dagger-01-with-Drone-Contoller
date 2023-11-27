#include <Servo.h>
Servo sT;Servo sR;Servo sL;

#define trigPin1 A0
#define echoPin1 A1
#define trigPin2 A2
#define echoPin2 A3

#define m1 2
#define m21 7
#define m22 4

#define ledR 11
#define ledG 12
#define ledB 13

long duration, distance, FIRSTSensor,SECONDSensor;

int timeGap = 10, ang;
void setup()
{delay(1000);
  sT.attach(5);
  sL.attach(6);
  sR.attach(3);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(m1, OUTPUT);
  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
  
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  
  sT.write(90);
  sL.write(180);
  sR.write(10);liftMotor(0);
 // disco2();
  //disco2();
  digitalWrite(m1, HIGH);
  
  
  Serial.begin(9600);
  delay(500);
}

void loop()
{

  sT.write(0);  
  
  for(int a=1; a<5; a++){
    ang = 90-(15*a);
    sL.write(ang);
    sR.write(ang);
    
    for(int i=0;i<180;i++){
        sT.write(i);
        

        delay(timeGap);
    
    }
    liftMotor(0);
    disco();
    a++;
    ang = 90-(15*a);
    sL.write(ang);
    sR.write(ang);

    for(int i=180;i>0;i--){
        sT.write(i);

        delay(timeGap);    
    }
    disco();
    liftMotor(1);
  }

}
void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}

void disco(){
  int gp = 200, pin = ledB;


  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW);

  pin = ledR;
  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW);

  pin = ledG;
  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW); 
}


void disco2(){
  int gp = 600, gp2 =500, pin = ledB;
  sL.write(180);
  sR.write(10);

  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW);

  sL.write(160);
  sR.write(30);
  pin = ledR;
  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW);

  sL.write(140);
  sR.write(50);
  pin = ledG;
  digitalWrite(pin, HIGH);
  delay(gp);
  digitalWrite(pin, LOW); 
}

void liftMotor( int x){
 ;/*
  int pin = m21;
  if(x){pin = m22;}
  
  digitalWrite(pin, HIGH);
 
  delay(1000);
  digitalWrite(pin, LOW);
*/
   
}
void Motor(){
 ;
  int pin = m1;

  
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);
}
