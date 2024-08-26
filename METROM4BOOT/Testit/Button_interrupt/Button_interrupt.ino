int ledGreen = 5;  // LED connected to digital pin 5
int ledYellow = 6;  // LED connected to digital pin 6
int ledRed = 7; // LED connected to digital pin 7
int Button1 = 4;    // pushbutton connected to digital pin 4
int Button2 = 3;    // pushbutton connected to digital pin 3
int Button3 =2;    // pushbutton connected to digital pin 2
volatile byte state_red = LOW;  //Punaisen LED-lampun tilatietomuuttuja, joka on käynnistyksessä 0.
volatile byte state_yellow = LOW;
volatile byte state_green = LOW;

void setup() {
  pinMode(ledGreen, OUTPUT);  // sets the digital pin 5 as output
  pinMode(ledYellow, OUTPUT);  // sets the digital pin 6 as output
  pinMode(ledRed, OUTPUT);  // sets the digital pin 7 as output
  pinMode(Button1, INPUT_PULLUP);    // sets the digital pin 4 as input
  pinMode(Button2, INPUT_PULLUP);    // sets the digital pin 3 as input
  pinMode(Button3, INPUT_PULLUP);    // sets the digital pin 2 as input
  attachInterrupt(digitalPinToInterrupt(Button1), blink_red, FALLING);  //Keskeytysmääritys painonapille 1, joka IO-tulona aiheuttaa keskeytyksen. Kutsutaan ohjelmaa blink_red. Missä kohtaa keskeytys tapahtuu (falling, rising tai change).
  attachInterrupt(digitalPinToInterrupt(Button2), blink_yellow, FALLING); //Keskeytysmääritys painonapille 2, joka IO-tulona aiheuttaa keskeytyksen. Kutsutaan ohjelmaa blink_yellow. Missä kohtaa keskeytys tapahtuu (falling, rising tai change).
  attachInterrupt(digitalPinToInterrupt(Button3), blink_green, FALLING);  //Keskeytysmääritys painonapille 3, joka IO-tulona aiheuttaa keskeytyksen. Kutsutaan ohjelmaa blink_green. Missä kohtaa keskeytys tapahtuu (falling, rising tai change).

}
//Ledit syttyvät ja sammuvat nappia painamalla.
//Pääohjelma.
void loop() {
  digitalWrite(ledRed, state_red);
  digitalWrite(ledYellow, state_yellow);
  digitalWrite(ledGreen, state_green);
}

//Kutsuttavat aliohjelmat = keskeytysfunktiot, jonne hypätään napin painalluksesta. Vähän koodia, muutoin voi tulla virhetilanne ja ohjelma voi kaatua.
void blink_red() {
  state_red = !state_red; //From LOW to HIGH.
}

void blink_yellow() {
  state_yellow = !state_yellow;
}

void blink_green() {
  state_green = !state_green;
}