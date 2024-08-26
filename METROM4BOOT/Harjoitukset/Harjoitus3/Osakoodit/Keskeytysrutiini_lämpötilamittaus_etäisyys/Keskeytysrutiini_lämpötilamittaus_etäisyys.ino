// Veera Tommila. veera.tommila@gmail.com
// Harjoitus 3. Lämpötilan, kosteuden ja etäisyyden mittaaminen keskeytysrutiinilla.
#include "Adafruit_SHTC3.h"
#include "Adafruit_VL53L0X.h"

const int PwrSht = 12;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();  //Oliomuuttuja  SHT:lle, jolla viitataan anturiin.
Adafruit_VL53L0X lox = Adafruit_VL53L0X();  //Lox on anturiin viittava olio.

int Button2 = 3;    // pushbutton connected to digital pin 3
volatile byte state_Button2 = LOW;  //Pitäisi olla napin tilatietomuuttuja, joka on käynnistyksessä 0.

void setup() {
  pinMode(PwrSht, OUTPUT);
  digitalWrite(PwrSht, HIGH);
  Serial.begin(115200);

  while (!Serial)
    delay(1);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHTC3 testaus ja Adafruit VL53L0X testaus.");
  if (! shtc3.begin()) {  // Jos anturia ei löydy, jumitetaan tähän.
    Serial.println("Ei löytynyt SHTC3-anturia.");
    while (1) delay(1);
  }
  if (!lox.begin()) {
    Serial.println(F("Ei voitu käynnistää VL53L0X-anturia"));
    while(1);
  }
  
  Serial.println("Löydettiin SHTC3-anturi"); // Jos käynnistys onnistuu. Ilmoitetaan anturin löytymisestä.    
  pinMode(Button2, INPUT_PULLUP);    // sets the digital pin 3 as input
  attachInterrupt(digitalPinToInterrupt(Button2), keskeytys, FALLING); //Keskeytysmääritys painonapille 2, joka IO-tulona aiheuttaa keskeytyksen. Kutsutaan ohjelmaa keskeytys. Missä kohtaa keskeytys tapahtuu (falling, rising tai change).
  // power
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));
// start continuous ranging
  lox.startRangeContinuous();
}

void loop() {
  digitalWrite(Button2, state_Button2);
}

void keskeytys(){
  state_Button2 = !state_Button2;  
  sensors_event_t humidity, temp;  
  shtc3.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data. Käydään tällä komennolla lukemassa kosteus ja lämpötila.  
  Serial.print("Lämpötila: "); Serial.print(temp.temperature); Serial.println(" °C.");  // Muutetaan lämpötila celcius-asteiksi.
  Serial.print("Suhteellinen kosteus: "); Serial.print(humidity.relative_humidity); Serial.println("%."); // Mitataan suhteellinen kosteus.
  Serial.print("Etäisyys mm: ");Serial.println(lox.readRange());  
}