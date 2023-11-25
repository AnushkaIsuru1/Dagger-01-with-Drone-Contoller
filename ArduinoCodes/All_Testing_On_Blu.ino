#include <Servo.h>
#include <SoftwareSerial.h>

Servo sT;Servo sR;Servo sL;
//Softwareserial HC06;

#define lamp 2
#define buz A5

#define elvUp 8
#define elvDown 9
#define rottor 7

#define trigPin1 A0
#define echoPin1 A1
#define trigPin2 A2
#define echoPin2 A3
#define laser 4

#define ledR 11
#define ledG 12
#define ledB 13

char c;

long duration,distance,FIRSTSensor,SECONDSensor;
int timeGap = 15, ang;

void setup() {
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);

  Serial.begin(9600);
  //HC06.begin(9600);
  sT.attach(5);
  sL.attach(6);
  sR.attach(3);

  pinMode(lamp,OUTPUT);
  pinMode(buz,OUTPUT);
  pinMode(elvUp,OUTPUT);
	pinMode(elvDown,OUTPUT);
  pinMode(rottor,OUTPUT);

  pinMode(trigPin1,OUTPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(echoPin2,INPUT);
  pinMode(laser,OUTPUT);

  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(ledB,OUTPUT);

  digitalWrite(elvUp, HIGH);  
  digitalWrite(rottor, HIGH);
  
}

void loop(){/*


  digitalWrite(lamp, HIGH);
  digitalWrite(rottor, HIGH);
  delay(2000);
  digitalWrite(lamp, LOW);
  digitalWrite(rottor, LOW);
  
  Elevetor(1);
  delay(5000);
  Elevetor(0);
  delay(5000);*/

  if(Serial.available()){
    c= Serial.read();
  }

  if(c=='1'){
    Elevetor(1);
    delay(300);
    Elevetor(0);
  }
  else if(c=='2'){
    Elevetor(2);
    delay(300);
    Elevetor(0);   
  }

  else if(c=='3'){
    Rotor(1);
    
  }else if(c=='4'){
    Rotor(0);
  }

  else if(c=='5'){
   digitalWrite(lamp,HIGH);    
  }else if(c=='6'){
    digitalWrite(lamp,LOW);
  }
  
  else if(c=='7'){
    Scaner();
  }else if(c=='8'){
    tone(buz, 4000, 200);
  }
  
  else if(c=='9'){
    
  }else if(c=='0'){
    
  }

  else if(c=='r'){
    LED('R');
  }else if(c=='g'){
    LED('G');
  }else if(c=='b'){
    LED('B');
  }
  c='\0';
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
void Scaner() {

  sT.write(0);  
  
  for(int a=1; a<5; a++){
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

       // Serial.print("1\t");
        Serial.print(FIRSTSensor);

        Serial.print("\t\t");
        Serial.println(FIRSTSensor);

        delay(timeGap);    
    }
  }

}
void Rotor(int x){
  if(x==1){
    digitalWrite(rottor, LOW);    
  }else{
    digitalWrite(rottor, HIGH);
  }
}

void Elevetor(int x){

  digitalWrite(elvUp, HIGH);
  digitalWrite(elvDown, LOW);

  if(x==1){
    digitalWrite(elvUp, LOW);
    digitalWrite(elvDown, LOW);
  }else if(x==2){
    digitalWrite(elvUp, HIGH);
    digitalWrite(elvDown, HIGH);
  }
}

void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(laser, LOW);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  //digitalWrite(laser, );
}