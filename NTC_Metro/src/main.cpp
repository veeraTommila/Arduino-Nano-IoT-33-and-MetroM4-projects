#include <Arduino.h>

int analogPin = A1; //
int pwrPin = A2;
int val = 0;  // variable to store the value read
void setup(){
  pinMode(pwrPin , OUTPUT);
  digitalWrite(pwrPin , HIGH);
  Serial.begin(9600);  // setup serial
}

void loop(){
  analogReadResolution(10);
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);  // debug value
  delay(2000);
}
