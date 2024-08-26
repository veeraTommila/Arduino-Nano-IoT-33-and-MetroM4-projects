/*************************************************** 
  This is an example for the SHTC3 Humidity & Temp Sensor

  Designed specifically to work with the SHTC3 sensor from Adafruit
  ----> https://www.adafruit.com/products/4636

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/

#include "Adafruit_SHTC3.h"

const int PwrSht = 12;

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();  //Oliomuuttuja  SHT:lle, jolla viitataan anturiin.

void setup() {
  pinMode(PwrSht, OUTPUT);
  digitalWrite(PwrSht, HIGH);

  Serial.begin(9600);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHTC3 test");
  if (! shtc3.begin()) {  // Jos anturia ei löydy, jumitetaan tähän.
    Serial.println("Couldn't find SHTC3");
    while (1) delay(1);
  }
  Serial.println("Found SHTC3 sensor"); // Jos käynnistys onnistuu. Ilmoitetaan anturin löytymisestä.
}


void loop() {
  sensors_event_t humidity, temp;
  
  shtc3.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data. Käydään tällä komennolla lukemassa kosteus ja lämpötila.
  
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");  // Muutetaan lämpötila celcius-asteiksi.
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH"); // Mitataan suhteellinen kosteus.

  delay(1000);
}