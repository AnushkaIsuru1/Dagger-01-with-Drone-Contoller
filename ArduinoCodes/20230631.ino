#include <SoftwareSerial.h>
SoftwareSerial HC06(0,1);
String msgr;
char c;
// int int6, int7, etat6 = LOW, etat7 = LOW;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  HC06.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
    if(HC06.available())
  {

    c=HC06.read();
    Serial.println(c);

  }
    if(c == 'a'){
      digitalWrite(12,HIGH);
      HC06.println('N');
      //HC06.print("LED2:1\r\n");
    }
    if(c == 'b')
    {
      digitalWrite(12,LOW);
      HC06.println('F');
      //HC06.print("LED2:1\r\n");
     }
  delay(1000);
}

