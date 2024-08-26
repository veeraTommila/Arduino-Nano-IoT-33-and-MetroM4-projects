#include <Arduino.h>
#include "Adafruit_VL53L0X.h"
/*
HARJOITUS 6   Veera Tommila
Metro M4 alusta ja siihen liitetyssä sensorilevyssä on yhtenä anturina VL53L0X laser sensori (Time of Light). 
Testikoodilla vl53l0x_continuous.ino saadaan alla olevan kuvan mukaista mittausta. Nähdään, että peräkkäisissäkin mittausarvoissa voi olla isojakin eroja, 
jopa 5mm eroja vaikka alusta ja kohde ovat vakaasti paikallaan.

Tehtävänä on muokata mittauskoodia niin, että se laskee jokaiseen tulostettavaan arvoon aina keskiarvon kymmenestä aiemmasta arvosta.
Käytä array tietotyyppiä, jossa on 10 kokonaisluku alkiota. Alusta array-tyyppinen muuttuja alussa arvoilla 8190, joka on anturin antama maksimiarvo.
Palauta kommentoitu koodi zipattuna Harjoitus 6 palautuslaatikkoon 20.11.2022 mennessä
*/
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int arvot[10] = {8190,8190,8190,8190,8190,8190,8190,8190,8190,8190};
int i = 10;
float keskiarvo = 0.0f;
float summa = 0;

//Keskiarvon laskemisen aliohjelman esittely.
void laskeKeskiarvo();

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

  // start continuous ranging
  lox.startRangeContinuous();
}

void loop() { 
  laskeKeskiarvo();  //Keskiarvon laskemisaliohjelman kutsu.
  if (lox.isRangeComplete()) {
    //Serial.print("Distance in mm: ");
    Serial.println(lox.readRange());
  }
  
}

//Aliohjelma keskiarvon laskemiseen.
void laskeKeskiarvo(){
  keskiarvo = 0.0f;
  summa = 0;

  Serial.println("Etäisyyden keskiarvo on: ");  //Tämän lauseen voi tulostaa joko ennen for-silmukkaa tai sen jälkeen.
  for (i = 0; i < 10; i++)
  {
    arvot[i] = lox.readRange();
    //Serial.println("Arvot:"); //Näillä vain testataan arvojen mittaus ja keskiarvon laskeminen. Näitä ei ole pakko tulostaa.
    //Serial.println(arvot[i]);
    summa = summa + arvot[i]; //Tallennetaan mitatut arvot.
    keskiarvo = summa / 10; //Keskiarvon laskeminen.
    //Keskiarvoa ei kannata tulostaa tässä! Tulos on väärä!
    //Serial.print("\n"); 
         
  }  
  //Serial.println("Etäisyyden keskiarvo on: ");
  Serial.println(keskiarvo); //Keskiarvon tulostus.    
  //delay(50000);  //Kuinka usein keskiarvo lasketaan. Tämä ei kuitenkaan ole välttämätön, mutta kätevä keskiarvon tuloksen tarkistamiseen, kun tulostetaan Arvot ja arvot[i].
}