int ledGreen = 5;  // LED connected to digital pin 5
int ledYellow = 6;  // LED connected to digital pin 6
int ledRed = 7; // LED connected to digital pin 7
int Button1 = 4;    // pushbutton connected to digital pin 4
int Button2 = 3;    // pushbutton connected to digital pin 3
int Button3 =2;    // pushbutton connected to digital pin 2

void setup() {
  pinMode(ledGreen, OUTPUT);  // sets the digital pin 5 as output
  pinMode(ledYellow, OUTPUT);  // sets the digital pin 6 as output
  pinMode(ledRed, OUTPUT);  // sets the digital pin 7 as output
  pinMode(Button1, INPUT_PULLUP);    // sets the digital pin 4 as input
  pinMode(Button2, INPUT_PULLUP);    // sets the digital pin 3 as input
  pinMode(Button3, INPUT_PULLUP);    // sets the digital pin 2 as input
}

void loop() {
  if(!digitalRead(Button1)){  //Jos nappia painetaan, lamppu syttyy. Luetaan Button1:sen tila. Ehto on tosi, jos tila on 0. 
    //val = digitalRead(Button1);   // read the input pin
    digitalWrite(ledRed, HIGH);     
  }
    else{
      digitalWrite(ledRed, LOW);  //Jos nappia ei paineta, lamppu on sammuksissa.
    }  // sets the LED to the button's value

  if(!digitalRead(Button2)){
    //val = digitalRead(Button1);   // read the input pin
    digitalWrite(ledYellow, HIGH);
  }
    else{
      digitalWrite(ledYellow, LOW);
    }  // sets the LED to the button's value

  if(!digitalRead(Button3)){
    //val = digitalRead(Button1);   // read the input pin
    digitalWrite(ledGreen, HIGH);
  }
    else{
      digitalWrite(ledGreen, LOW);
    }  // sets the LED to the button's value    
  }
  
