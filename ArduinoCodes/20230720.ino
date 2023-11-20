#include <Servo.h>

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

int landScannerState = 0,
    autoLandingState = 0, 
    proximityMapState = 0, 
    gpwsState = 0, 
    levelOfLand = 0, 
    inputCounter = 0,
    isFlying = 0;


void setup() {  

  Serial.begin(9600);

  sT.attach(5);
  sL.attach(6);
  sR.attach(3);

  pinMode(echoPin1,INPUT);
  pinMode(echoPin2,INPUT);
  pinMode(trigPin1,OUTPUT);
  pinMode(trigPin2,OUTPUT);

  pinMode(lamp,OUTPUT);
  pinMode(buz,OUTPUT);
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

  topServoForGPWS(0);
  Serial.println("PW");
  delay(400);
  
}
void feedbackLoop(){
  inp='\n';
  if(Serial.available()){
    inp = Serial.read();
    inputCounter = 0;
  }else{
    inputCounter++;
    if(inputCounter == 60  * 1000 && isFlying){      
      Serial.println("AL1");
      LED('G');
      autoLanding();
    }
  }
  switch (inp)
  {
  case '0':
    ElevatorDown();
    break;

  case '1':
    ElevatorUp();
    break;

  case '2':
    ElevatorUpAuto();
    break;

  case '3':
    ElevatorDownAuto();
    break;

  case '4':
    ScannerStateReset();
    launch();
    break;

  case '5':
    ScannerStateReset();
    land(); 
    break;

  case '6':
    ScannerStateReset();
    delayLED('R',0,0,0);
    LED('B');
    Serial.println("LS");
    landScannerState = 1;     
    break;

  case '7':
    ScannerStateReset();    
    Serial.println("AL");
    autoLandingState = 1;    
    LED('G');
    break;

  case '8':
    ScannerStateReset();
    delayLED('R',0,0,0);
    LED('B');
    Serial.println("PM");
    proximityMapState = 1;     
    break;

  case '9':
    ScannerStateReset(); 
    delayLED('R',0,0,0);
    LED('B');
    Serial.println("GP");
    gpwsState = 1; 
    break;

  case 'A':
    Lamp(1);
    break;

  case 'a':
    Lamp(0);
    break;

  case 'B':
    Laser(1);
    break;

  case 'b':
    Laser(0);
    break;

  case 'C':
    Rotor(1);
    break;

  case 'c':
    Rotor(0);
    break;

  case 'D':
    Tone(1);
    break;
  
  case '.':
    ScannerStateReset();
    Serial.println("RS");
    break;
  }
  case 'L':
    delayLED
    break;
  }

  if(gpwsState!=0){
    gpws();
  }
  else if(autoLandingState!=0){
    autoLanding();
  }
  else if(landScannerState!=0){
    landScanner();
  }
  
}

void loop() {
  feedbackLoop();
  delay(1);
}

void ScannerStateReset(){
  landScannerState = 0;
  autoLandingState = 0;
  proximityMapState = 0; 
  gpwsState = 0; 
}

//=============================================================  *  *  *  *  GPWS  *  *  *  *  =============================================================

void gpws(){
  digitalWrite(laser, HIGH);
  rotateServoSameDirection(10);

  int rotateFrom=30, rotateTo=140,rotateTimeGap = 10, loopTimeGap = 7, toneDelay=-1;
  long diagonal;

  if(gpwsState%2==0){
    rotateFrom=130, rotateTo=50;      
  }
  
  while(1){
    long temp = 0;
    
    if(gpwsState%2==1){
      for(int i = 1; i<=10; i++){
        topServoForGPWS(rotateFrom++);
        delay(loopTimeGap);
        if(temp<getGPWSData()){
          temp = getGPWSData();
        }
        delay(loopTimeGap);
      }
    }
    else{
      for(int i = 1; i<=10; i++){
        topServoForGPWS(rotateFrom--);
        delay(loopTimeGap);
        if(temp<getGPWSData()){
          temp = getGPWSData();
        }
        delay(loopTimeGap);
      }
    }  

    char sep[3] = ",";
    Serial.print("GG,");    
    Serial.print(rotateFrom);
    Serial.print(sep);
    Serial.println(temp);
    
    toneDelay++;

    if(temp>15 && temp<100){ 
      
      if(toneDelay + 20 < rotateFrom || toneDelay - 20 > rotateFrom){
        Tone(0); 
        toneDelay = rotateFrom; 
      }     
          
      LED('G');
      if(temp<60){
        LED('R');
      }
    }else{
      LED('B');
    }
    if(rotateFrom==30 || rotateFrom==140){
        break;
    }
      delay(rotateTimeGap);
  }

  gpwsState++;

  if(gpwsState==3){
    gpwsState = 1;
  }
 
}

//=============================================================  *  *  *  *  Land Scanner *  *  *  *  =============================================================

void landScanner(){
  
  int angle, rotateFrom=0, rotateTo=180, increment = 10,rotateTimeGap = 15;
  long diagonal, errorRange = 10;
  if(landScannerState%2==0){
    rotateFrom=180, rotateTo=0;increment = -10;    
  }
  if(landScannerState==1 || landScannerState==5){
    if(landScannerState==1 ){
      levelOfLand=0;
      getVerticalLevel(0);
    }
    
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
    while(1){
      long tempL = 0, tempR=0;

      if(landScannerState%2==1){
        for(int i = 1; i<=10; i++){
          topServo(rotateFrom++);
          delay(10);
          tempL += getUltrasonicData('L');
          tempR += getUltrasonicData('R');
          delay(15);
        }
      }
      else{
        for(int i = 1; i<=10; i++){
          topServo(rotateFrom--);
          delay(10);
          tempL += getUltrasonicData('L');
          tempR += getUltrasonicData('R');
          delay(15);
        }
      }    
    
    leftSensor = tempL /= 10;
    rightSensor = tempR /=10;

    
/* tempL = tempR = 0;
  if(((diagonal - errorRange) > leftSensor) && (leftSensor > (errorRange + diagonal))){
      tempL = leftSensor;
    }

    if((diagonal - errorRange) > rightSensor && rightSensor > (errorRange + diagonal)){
      tempR = rightSensor;
    }tempL = leftSensor;
    tempR = rightSensor;*/ 
    
     if((((diagonal - errorRange) < leftSensor) && (leftSensor < (errorRange + diagonal)) )){
      tempL = 0;
    }

    if((((diagonal - errorRange) < rightSensor) && (rightSensor < (errorRange + diagonal)) )){
      tempR = 0;
    }

    if(tempL != 0 || tempR!=0){
      levelOfLand +=1;
    }
    char sep[3] = "\t\t";
    Serial.print("GS-");
    Serial.print(landScannerState);  Serial.print(sep);
    
    Serial.print(rotateFrom/10);     Serial.print(sep);

    Serial.print("V - ");            
    Serial.print(verticalLevel);    Serial.print(sep);

    Serial.print("L - ");            
    Serial.print(leftSensor);        Serial.print(sep);

    Serial.print("R - ");            
    Serial.print(rightSensor);       Serial.print(sep);    
        
    Serial.print(tempL);             Serial.print(sep); 
       
    Serial.println(tempR);

    if(rotateFrom==0 || rotateFrom==180){
      break;
    }
    delay(rotateTimeGap);
  }
  landScannerState++;
  if(landScannerState==9){
    landScannerState = 1;
  }
}


//=============================================================  *  *  *  *  Ultrasonic  *  *  *  *  =============================================================

long getVerticalLevel(int t){
  
  sT.write(90);
  rotateServoSameDirection(90);
  delay(60);
  leftSensor = getUltrasonicData('L');
  rightSensor = getUltrasonicData('R');

  Serial.print(leftSensor);
  Serial.print("\t\t");
  Serial.println(rightSensor);
  
  if(leftSensor>rightSensor){
    verticalLevel = leftSensor;
  }else {
    verticalLevel =rightSensor;
  }
  Serial.print("AT");
  Serial.println(verticalLevel);
  return verticalLevel;
}
long getGPWSData(){
  
  delay(1);
  leftSensor = getUltrasonicData('L');
  rightSensor = getUltrasonicData('R');
  //Serial.print(leftSensor);Serial.print("\t\t");Serial.print(rightSensor);Serial.println("\t\t");
/*
  
  Serial.print("\t\t");
  Serial.println(rightSensor);*/
  delay(0);
  if(leftSensor>rightSensor){
    return leftSensor;
  }else {
    return rightSensor;
  }
 
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
  delay(0);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  digitalWrite(laser, HIGH);

  return (duration/2) / 29.1;  
}

//=============================================================  *  *  *  *  Launch  *  *  *  *  =============================================================

void autoLanding(){
  if(autoLandingState==1){
    landScannerState = 1;    
    landScanner();    
  }
  else if(autoLandingState<4){
    landScanner();
  }
  else if(levelOfLand==0){
    autoLandingState = -1;
    landScannerState = 0;
    land();
  }
  else if(autoLandingState==4){
    autoLandingState = 0;
    autoLandingState=1;    
  }else{
    
  }
  autoLandingState++;
}

void launch(){
  int timeGap = 700;
  Tone(1);
  LED('R');
  delay(1500);

  Laser(1);
  delay(500);

  LED('G');
  topServoForGPWS(90);
  delay(1500);

  rotateServoSameDirection(0);
  delay(2000);

  LED('B');
  Lamp(1);
  delay(1000);
  
  Rotor(1);
  delay(3000);
  ElevatorUpAuto();
  isFlying = 1;
  Serial.println("LH");
}

void land(){
  Tone(1);
  ElevatorDownAuto();
  delay(5000);  
  
  Rotor(0);
  delay(5000);

  Lamp(0);
  delay(1000);

  rotateServoSameDirection(90);
  delay(1000);

  topServoForGPWS(0);
  delay(1000);

  Laser(0);
  delay(500);
    Serial.println("LD");
  Tone(0);
  LED('0');
}

//=============================================================  *  *  *  *  Elevator Controll  *  *  *  *  =============================================================

void ElevatorUpAuto(){
  int ElevatorUpAutoCounter = 0;
  ElevatorOnDelay(1, 6000);

  while(1){  
    delay(50);

    if((getVerticalLevel(0)> 50)){
      break;
    }else if(ElevatorUpAutoCounter>5){
      break;
    }
    else{
      ElevatorOnDelay(1, 600); 
      delay(1000);
    }
    ElevatorUpAutoCounter++;
  }

}

void ElevatorDownAuto(){
  ElevatorOnDelay(0, 300);
  delay(1500);

  ElevatorOnDelay(0, 300);
  delay(2000);

  while(1){
    
    if(getVerticalLevel(0)>10){
      ElevatorOnDelay(0, 50);
      delay(100);
      continue;
    }
    delay(50);
    ElevatorOnDelay(1, 300);
    break;
  }
  isFlying = 0;
}

void ElevatorUp(){
  ElevatorOnDelay(1, 0);
}

void ElevatorDown(){
  ElevatorOnDelay(0, 0);
 
}

void ElevatorOnDelay(int x, int workDelay){
  if(workDelay==0){
    workDelay = 300;
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

//=============================================================  *  *  *  *  Servo  *  *  *  *  =============================================================

void topServo(int a){
  sT.write(a + 0);
  
}
void topServoForGPWS(int a){
  sT.write(a + 10);
  
}

void rotateServo(int a){
  Serial.print("SS,");
  Serial.println(a);

  sR.write(a + 10);
  sL.write(a-5);
}

void rotateServoSameDirection(int a){
  Serial.print("SS,");
  Serial.println(a);
  
  sR.write(a + 10);
  sL.write(180 - a-5);
}

//=============================================================  *  *  *  *  Component ON-OFF *  *  *  *  =============================================================

void Rotor(int x){
  if(x==1){
    digitalWrite(rottor, LOW);    
    Serial.println("R1");
  }else{
    digitalWrite(rottor, HIGH);
    Serial.println("R0");
  }
}

void Laser(int x){
  if(x==1){
    digitalWrite(laser, HIGH);    
    Serial.println("X1");
  }else{
    digitalWrite(laser, LOW);
    Serial.println("X0");
  }
}

void Lamp(int x){
  if(x==1){
    digitalWrite(lamp, HIGH);    
    Serial.println("L1");
  }else{
    digitalWrite(lamp, LOW);
    Serial.println("L0");
  }
}

void delayLED(char x, int c, int o, int d){
  if(c==0){
    c = 3;
  }
  if(o==0){
    o = 50;
  }
  if(d==0){
    d = 120;
  }

  for(int i=0; i<c;i++){
    LED(x);
    delay(o);
    LED('0');
    delay(d);
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

void Tone(int t){
  if(t==0){
    tone(buz, 4000, 200);    
  }
  else if(t==1){
    tone(buz, 4000, 200);    
    delay(350);
    tone(buz, 4000, 200); 
  }
}

void shutDown(){
  Rotor(0);
  Lamp(0);
  Laser(0);
  topServoForGPWS(0);
  rotateServoSameDirection(90) ;
  LED('a');
}