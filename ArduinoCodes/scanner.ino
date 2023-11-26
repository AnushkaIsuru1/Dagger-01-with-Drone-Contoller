#include <Servo.h>
Servo sT;Servo sR;Servo sL;

#define trigPin1 A0
#define echoPin1 A1
#define trigPin2 A2
#define echoPin2 A3
#define m1 12
#define m2 13

long duration, distance, FIRSTSensor,SECONDSensor;

int timeGap = 15, ang;
void setup()
{
  sT.attach(3);
  sL.attach(5);
  sR.attach(6);
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  Serial.begin(9600);
/*
  sT.write(90);
  sL.write(0);
  sR.write(180);

  delay(2000);*/
}

void loop()
{

  sT.write(0);  
  
  for(int a=1; a<2; a++){
    ang = 90-(15*a);
    sL.write(ang);
    sR.write(ang);

    for(int i=0;i<180;i++){
        sT.write(i);
        
        SonarSensor(trigPin1, echoPin1);
        FIRSTSensor = distance;

        SonarSensor(trigPin2, echoPin2);
        SECONDSensor = distance;

        //Serial.print("1\t");
        Serial.print(FIRSTSensor);

        Serial.print("\t\t");
        Serial.println(SECONDSensor);

        delay(timeGap);
    
    }
    a++;
    ang = 90-(15*a);
    sL.write(ang);
    sR.write(ang);

    for(int i=180;i>0;i--){
        sT.write(i);
        
        SonarSensor(trigPin1, echoPin1);
        FIRSTSensor = distance;

        SonarSensor(trigPin2, echoPin2);
        SECONDSensor = distance;

        Serial.print("1\t");
        Serial.print(FIRSTSensor);

        Serial.print("\t\t");
        Serial.println(FIRSTSensor);

        delay(timeGap);    
    }
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
