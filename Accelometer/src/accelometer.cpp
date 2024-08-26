#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <WiFiUdp.h>
#include "Adafruit_TSL2591.h"
#include <Wire.h>
#include <Arduino_LSM6DS3.h>
/*Harjoitus 9. Veera Tommila.
C-ohjelma ulkolämpötilan ja valoisuuden mittaamiseen sekä liikkeen tunnistukseen ja hälytyksen kuittaukseen.

Luo Asksensorsin pilvipalveluun edellisessä harjoituksessa luotuun Sensor2:een seuraavat moduulit ja graafit:
module3 (liike, Binary) module4 (liike, Table)
Jatketaan harjoitus 8:n koodia seuraavasti
Seuraava koodi mittaa kiihtyvyyttä (liike) ja tulostaa sen arvon, jos se on yli 1.5 (testaa omalla laitteella mikä on sopiva) ja muuten tulostaa nollan. 
Lisää tästä koodista tarvittavat osat omaan koodiisi (tämä esimerkkikoodi löytyy myös lisämateriaalikansiosta nimelläLSM6DS3_esimerkki.zip). 
Laitteen pitäisi toimia siten, että aina kun laite tunnistaa liikkeen esimerkkikoodin arvoilla niin pilveen lähetetään arvo 1 ja levyllä oleva LED (pinni D6) sytytetään. 
Painonapilla (Button1 pinnissä D7) voidaan hälytys kuitata ja silloin lähetetään pilveen arvo 0 ja myös LED sammutetaan.
Jos painonappi ei toimi heti, paina sitä useamman kerran.

Aliohjelmien esittely.*/
void mittaa();
void printWifiStatus();
void haeJson();
void httpRequest();
void liike();
void kuittaus();

Adafruit_BME280 bme;  //BME280-anturin ilmentymä, jota käytetään anturitietojen lukemiseen.
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); //TSL2591-anturin ilmentymä, jota käytetään anturitietojen lukemiseen.

float jsonTuulenNopeus;
float jsonTuulenSuunta;
double jsonLampotila;  //Älä laita ääkkösiä muuttujien nimiin, Jason ei pidä siitä!
int jsonTimeZone;
const char* jsonKaupunki;
unsigned int localPort = 2390;
uint16_t ir, full;
uint32_t luminosity;
volatile int mov1;
volatile int kuitattu = 1; 
//int status;
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 7; // the number of the pushbutton pin
/*const int ledPinR = 5; // the number of the RED LED pin
const int ledPinG = 6; // the number of the GREEN LED pin*/
// variables will change:
bool ledStateG = HIGH;
int counter = 0;
long int alku, loppu, aika; // 32-bittiset aikamuuttujat.

//Wifiverkon tietoja.
char ssid[] = "Zyxel_D2D1";        // your network SSID (name). Kotiverkon nimi.
char pass[] = "MAG4HKLH3K";    // your network password (use for WPA, or use as key for WEP). Kotiverkon salasana.
char apikey2[] = "Jg6VmHTCsywxQimr5aJIjRxweHbYkn9u"; //apikey of first sensor. Tähän toisen sensorin.
int keyIndex = 0;            // your network key index number (needed only for WEP)

//Json datan serveri ja client
char jsonServer[] = "api.openweathermap.org";
WiFiClient jsonClient;  //Luodaan wificlient-tyyppinen muuttuja nimeltä jsonClient.

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

int status = WL_IDLE_STATUS;  // Status-muuttuja, joka on määritelty WL_IDLE_STATUkseksi.

// Initialize the WiFi asiakas library
WiFiClient asiakas;

// server address:
char server[] = "asksensors.com";

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60L * 1000L; // Lähetetään tietoa minuutin välein.

void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

void setup() {
  pinMode(6, OUTPUT); // initialize the RED LED pin as an output
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial); 
  if (!IMU.begin()) { 
    Serial.println("Failed to initialize IMU!"); 
    while (1); 
  } 
  Serial.print("Accelerometer sample rate = "); 
  Serial.print(IMU.accelerationSampleRate()); 
  Serial.println(" Hz");

  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  
if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }

  while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
    bme.begin(0x76);
  }

  configureSensor();

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
  Udp.begin(localPort);
}

void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir), 6);
}

/**************************************************************************/
/*
    Performs a read using the Adafruit Unified Sensor API.
*/
/**************************************************************************/
void unifiedSensorAPIRead(void)
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Invalid data (adjust gain or timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}

  void loop(){    
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (asiakas.available()) {  //Jos yhteys on olemassa.
    char c = asiakas.read(); //Vastaanotetaan merkkejä palvelimelta, tallennetaan ne muuttujaan c ja tulostetaan.
    Serial.write(c);
  }
    /*Kutsutaan funktiota haeJson, tehdään sen hommat, palataan takaisin, 
    odotetaan 30 sekuntia ja tehdään sama uudelleen. */
    //After 1 min, get sensor readings and connect to server:
  if ((millis() - lastConnectionTime) > postingInterval) {    
    mittaa();
    haeJson();
    httpRequest(); //request function for all the other sensors
    //Liikkeen koodit tähän, jos void liikee()-kutsuminen ei toimi.   
  }
   liike();
   kuittaus();
  //delay(30000);
  }

  // Tämä funktio on mittauksia varten.
void mittaa(){
    //Mittauksia tulee, jos laittaa setup()-funktioon bme.begin(0x76)nin Serial.begin(9600)nin jälkeen tai while(!Serial)-silmukkaan.    
    luminosity = tsl.getFullLuminosity();
    ir = luminosity >> 16;
    full = luminosity & 0xFFFF;    
    luminosity = tsl.calculateLux(full, ir)* 1.0f;
    
    // compare the previous status to the current status
    if (status != WiFi.status()) {
      // it has changed update the variable
      status = WiFi.status();

      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
      } else {
        // a device has disconnected from the AP, and we are back in listening mode
        Serial.println("Device disconnected from AP");
    }
  }
}

void liike(){
  float x, y, z, g; 
    if (IMU.accelerationAvailable()) { 
      IMU.readAcceleration(x, y, z); 
      g = sqrt(x*x+y*y+z*z); 
      mov1 = g;
      if(mov1 > 1.5f){
        //Serial.println(mov1);
        digitalWrite(6, HIGH);
        kuitattu = 1;
        //Serial.println(g); 
      } else{        
        //digitalWrite(5, LOW);
        //Serial.println(0);
      }
    }
}

void kuittaus(){
  if(digitalRead(buttonPin)== LOW){ //Jos luetun painonapin tila on LOW (arvo on 0). Se menee nollille painonappia painettaessa.  
    kuitattu = 0;    
    digitalWrite(6, LOW); // Sammutetaan vihreä LED.
  }
}

//Aliohjelmassa haetaan Json tietue muuttujaan
void haeJson(){
  //luminosity;
 Serial.println("\nStarting connection to JSONserver...");
  // if you get a connection, report back via serial:
  if (jsonClient.connect(jsonServer, 80)) {
    Serial.println("connected to server");
    
    // Send HTTP request. Lähetetään get-pyyntö, isännän nimi tekstinä ja connection-tieto, että yhteys suljetaan.
    //jsonClient.println(F("GET /data/2.5/weather?q=Kokkola&appid=aa65788a0c7617082906de01f740ad35&units=metric HTTP/1.0"));  //Kokkolan lämpötila.
    jsonClient.println(F("GET /data/2.5/weather?q=Vaasa&appid=3ad47f513f5e580203d5790e72d26e16&units=metric HTTP/1.0"));  //Vaasan lämpötila. 
    jsonClient.println(F("Host: openweathermap.org"));
    jsonClient.println(F("Connection: close"));
    if (jsonClient.println() == 0) {
      Serial.println(F("Failed to send request"));
      jsonClient.stop();
      return;
    }
    
    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!jsonClient.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      jsonClient.stop();
      return;
    }

    // Allocate the JSON document
    // Use https://arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = 1024;  //laskettu assistantilla
  
    DynamicJsonDocument doc(capacity);

    // Parse JSON object. Json-olion purku. Siten voidaan halutut tiedot poimia.
    DeserializationError error = deserializeJson(doc, jsonClient);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      jsonClient.stop();
      return;
    }

    // Extract values
    JsonObject main = doc["main"];
    jsonTuulenNopeus = doc["wind"]["speed"];
    jsonTuulenSuunta = doc["main"]["deg"];
    jsonTimeZone = doc["timezone"];
    jsonKaupunki = doc["name"];
    jsonLampotila = main["temp"];  //Reaaliaikaisen lämpötilan haku.
    Serial.println();
    Serial.println("Haun tulokset:");
    Serial.print("Lämpötila: ");
    Serial.println(jsonLampotila);    
    Serial.print("Kaupunki: ");
    Serial.println(jsonKaupunki);
    Serial.print("Paikallinen valoisuus: ");
    Serial.println(luminosity);
    Serial.print("Liike: ");
    Serial.println(mov1);

    // Disconnect
    jsonClient.stop();
      
  }
}

  //	This method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the NINA module
  asiakas.stop();

  // if there's a successful connection:
  if (asiakas.connect(server, 80)) {
    Serial.println("connecting...");

    String url = "https://asksensors.com/api.asksensors/write/"; //For sensor
    url += apikey2;
    url += "?module1="; //Valoisuus: line
    url += luminosity;
    url += "&module2="; //Lämpötila: line
    url += jsonLampotila;    
    url += "&module3="; //kuitattu: binary
    url += kuitattu;
    url += "&module4="; //mov1: line
    url += kuitattu;
    
    // send the HTTP GET request:
    Serial.print("********** requesting URL: ");
      Serial.println(url);
    asiakas.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n");
    asiakas.println(); 
    
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

  void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
