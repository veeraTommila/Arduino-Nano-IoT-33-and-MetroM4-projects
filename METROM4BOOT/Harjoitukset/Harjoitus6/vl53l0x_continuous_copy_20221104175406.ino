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
  keskiarvo = 0.0f;
  summa = 0;

for (i = 0; i < 10; i++)
      {
        arvot[i] = lox.readRange();
        //Serial.println("Arvot:");
        //Serial.println(arvot[i]);
        summa = summa + arvot[i];        
      }
      
      keskiarvo = summa / 10;
      Serial.println("Keskiarvo: ");
      Serial.println(keskiarvo); 

      //delay(10); 
    
  if (lox.isRangeComplete()) {
    //Serial.print("Distance in mm: ");
    Serial.println(lox.readRange());
  }
}