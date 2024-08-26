#include <Arduino.h>

#include <NTPClient.h>
#include <SPI.h>
#include <WIFININA.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const char *ssid     = "ChydeniusOPEN";
const char *password = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Adafruit_BME280 bme; // I2C

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60000; // delay 1 min between updates, in milliseconds
float T, P, H;
unsigned int ero;

void send();

void setup(){
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  unsigned status;
    
    // default settings
    status = bme.begin(0x76);  
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

  timeClient.begin();
  delay(5000);

  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getEpochTime());
  
  lastConnectionTime = millis();
}

void loop() {


  if((millis() - lastConnectionTime) > postingInterval){
    T = bme.readTemperature();
    P = bme.readPressure() / 100.0F;
    H = bme.readHumidity();  
    send();
  }
  
}

void send(){
  timeClient.update();

  Serial.println();
  Serial.print(timeClient.getFormattedTime());
  Serial.print("   Lämpötila on: ");
  Serial.println(T);
  Serial.print(timeClient.getFormattedTime());
  Serial.print("   Ilmanpaine on: ");
  Serial.println(P);
  Serial.print(timeClient.getFormattedTime());
  Serial.print("   Kosteus on: ");
  Serial.println(H);
  Serial.println();
  
  lastConnectionTime = millis();
}