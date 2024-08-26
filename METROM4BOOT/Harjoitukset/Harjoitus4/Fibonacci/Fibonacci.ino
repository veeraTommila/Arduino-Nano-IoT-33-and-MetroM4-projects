/* Veera Tommila. veera.tommila@gmail.com
Harjoitus 4. Fibonacci-luvut: 0,1,1,2,3,5,8,13,21,....*/

/*
#include "Adafruit_SHTC3.h"
#include "Adafruit_VL53L0X.h"

const int PwrSht = 12;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();  //Oliomuuttuja  SHT:lle, jolla viitataan anturiin.
Adafruit_VL53L0X lox = Adafruit_VL53L0X();  //Lox on anturiin viittava olio.

int ledRed = 7; // LED connected to digital pin 7
int Button2 = 3;    // pushbutton connected to digital pin 3
volatile byte state_Button2 = LOW;  //Pitäisi olla napin tilatietomuuttuja, joka on käynnistyksessä 0.
volatile byte state_red = LOW;  //Punaisen LED-lampun tilatietomuuttuja, joka on käynnistyksessä 0.
*/

int n1=0,n2=1,summa,i;
int number = 8; 
void setup() {
  //pinMode(PwrSht, OUTPUT);
  //pinMode(ledRed, OUTPUT);  // sets the digital pin 7 as output
  //digitalWrite(PwrSht, HIGH);
  Serial.begin(9600);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens  
  //Serial.println("Löydettiin SHTC3-anturi"); // Jos käynnistys onnistuu. Ilmoitetaan anturin löytymisestä.    
  //pinMode(Button2, INPUT_PULLUP);    // sets the digital pin 3 as input
  
}

void loop() {
  //digitalWrite(ledRed, state_red);
  //digitalWrite(Button2, state_Button2);  
  
  //Tulostetaan Fibonaccin kahdeksan lukua tässä.
  while(summa <= 20){
    Serial.println("Fibonaccin 8 lukua ovat: ");
    for(i = 0; i < number; i++){
      summa=n1+n2;          
      n1=n2;    
      n2=summa;
      Serial.println(summa);      
    }
  }  
         
  //exit(0);  
  //delay(10000);
}


