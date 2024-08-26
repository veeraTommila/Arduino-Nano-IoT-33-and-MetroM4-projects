#include <Arduino.h>
#include <NTPClient.h>
#include <SPI.h>
#include <WIFININA.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
/* Veera Tommila
HARJOITUS 10 Tässä harjoituksessa käytetään NTP-protokollaa (Network Time Protocol), 
joka lähettää jollekin aikapalvelimelle UDP -pohjaisen aikakyselyn. 
Aikapalvelin vastaa ns. Epoch -ajalla, joka on aika sekunneissa, joka on kulunut 
alkuhetkestä (1.1.1970) tähän hetkeen asti. Käytetty kirjasto voi sitten laskea tästä 
aikoja esitettäväksi eri muodoissa esim. normaalisti muotoiltu kellonaika hh:mm:ss. 
Oppimisympäristön lisämateriaalit kansiosta löytyy esimerkkikoodi NTP_esim.zip, jossa 
BME280 sensorilla mitataan lämpötilaa, ilmanpainetta sekä kosteutta ja nämä lähetetään 
(tulostetaan) minuutin välein. Luo itsellesi uusi projekti ja kopioi tämä main.cpp 
tiedostoon. Muuta koodia niin, että lähetys tapahtuu aina tasaminuutein.
Palauta kommentoitu koodi zipattuna Harjoitus 10 palautuslaatikkoon 11.12.2022 mennessä.*/

const char *ssid     = "Zyxel_D2D1";  // Your local network's name.
const char *password = "MAG4HKLH3K";  // Your local network's password.

WiFiUDP ntpUDP; // NTP-instance.
//NTPClient timeClient(ntpUDP); // NTP-client instance timeClient with UDP-parameter.
NTPClient timeClient(ntpUDP,"0.fi.pool.ntp.org",3600, 60000);
Adafruit_BME280 bme; // I2C. Sensor instance.

unsigned long lastConnectionTime = 0; // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60000; // delay 1 min between updates, in milliseconds
float T, P, H;  // Temperature, pressure and humidity.
unsigned int ero; // The difference.

void send();

void setup(){
  Serial.begin(9600);
  
  WiFi.begin(ssid, password); // Wifi initialization.

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  unsigned status;
    
    // default settings
    status = bme.begin(0x76);  // BME-sensor initialization.
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

  timeClient.begin(); // timeClient initialization.
  timeClient.setTimeOffset(7200); // To determine the time zone or difference in seconds.  
  
  delay(5000);  // Time to do everything.

  timeClient.update();  // Use this allways before sending information query to time server.
  ero = 60-timeClient.getEpochTime()%60;  // Put the difference counter here to count seconds to the next full minute. 3600 is for hours.
  Serial.println(timeClient.getFormattedTime());  // Print queried time in hours, minutes and seconds.
  Serial.print("Kuinka monta sekuntia seuraavaan tasaminuuttiin: ");
  //Serial.println(60-timeClient.getEpochTime()%60);
  Serial.println(ero);  // Print the difference time.  
  lastConnectionTime = millis();  // Save the current time of the node.
  
}

void loop() {
  // Read and send the information in every minute.
  // Postinginterval is replaced by another variable.
  // Watch the time.
  if(timeClient.getSeconds()==0){
    T = bme.readTemperature();
    P = bme.readPressure() / 100.0F;
    H = bme.readHumidity();  
    send();
    //delay(postingInterval);
  } 
  //delay(postingInterval);
}

//Share the function to hours, minutes and seconds. 
//Print when second is 0 and check that the value is actually 0.

// To measure the current time and values in exact on minute.
void send(){
  timeClient.update();  // To read time values from the time server.
  //timeClient.setTimeOffset(ero); // Get the difference.
  timeClient.setTimeOffset(7200); // Get the time zone.
  timeClient.getHours();
  timeClient.getMinutes();
  timeClient.getSeconds();
// To print the values on the exact minute.
if(timeClient.getSeconds()==0){    
    //Serial.println();
    Serial.print(timeClient.getFormattedTime());  // The current time of the node.
    Serial.print("   Lämpötila on: ");  // Temperature.
    Serial.println(T);
    Serial.print(timeClient.getFormattedTime());
    Serial.print("   Ilmanpaine on: "); // Pressure.
    Serial.println(P);
    Serial.print(timeClient.getFormattedTime());
    Serial.print("   Kosteus on: ");  // Humidity.
    Serial.println(H);
    Serial.println();
    timeClient.update();
    ero = 60-timeClient.getEpochTime()%60;  // Recount the difference. 
    lastConnectionTime = millis();  // Return back to the main program.
  }
  delay(1000); 
}