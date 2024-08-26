/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will create a new access point (with no password).
  It will then launch a new server and print out the IP address
  to the Serial Monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 13.

  If the IP address of your board is yourAddress:
    http://yourAddress/H turns the LED on
    http://yourAddress/L turns it off

  created 25 Nov 2012
  by Tom Igoe
  adapted to WiFi AP by Adafruit
 */

#include <SPI.h>  // WifiNINA keskustelee mikro-ohjaimen kanssa SPI-väylän kautta.
#include <WiFiNINA.h>
#include <WiFiUdp.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "VeeranAP";        // your network SSID (name)
char pass[] = "12345678_Qwertyuiopå!";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key index number (needed only for WEP)

const int analogPin = A6; // Analogisen anturin NTC:n vastus.
const int pwrPin = 17;  // Syötetään voimat pinnistä 17 pinniin A6.
int adcValue = 0;  // variable to store the (ADC) value read
  
unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP timestamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;



int status = WL_IDLE_STATUS;
WiFiServer server(80);  //Wifi-palvelimen alustus. Parametrina on kuunneltava portti (nyt HTTP-portti).

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(pwrPin, OUTPUT);    //  pinni D17 lähdöksi
  digitalWrite(pwrPin, HIGH); //  virrat päälle sensorille 

  Serial.println("Access Point Web Server");  // Tukiaseman tekeminen verkkopalvelimesta.
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {  // Jos luettava statustieto on WL_NO_MODULE, tulee virheilmoitus ja jumiudutaan tähän.
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true); // Ollaan tässä niin kauan, kunnes tilanne muuttuu.
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);  // Käynnistetään verkko näillä parametreillä.
  if (status != WL_AP_LISTENING) {  // Statusarvon tarkistus.
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);
}


void loop() {

  // compare the previous status to the current status
  if (status != WiFi.status()) {  // Jos edellinen tilatieto on eri kuin nyt. Päivitetään statustieto vastaamaan nykyisen statuksen tietoa.
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {  // Onko jokin laite kytkeytynyt tukiasemaan.
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

    mittaaADC();

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           
           // CSS to style

            // Web Page Heading
            client.println("</head><body><h1>NANO33 Server</h1>");
            client.println("ADC arvo on: ");
            client.println(adcValue);
            client.println("</body></html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());  // Haetaan SSID:n arvon.

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip); // Mikä IP-osoite alustalla on.

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void mittaaADC(){
  adcValue = analogRead(analogPin);  // luetaan A6:n jännitearvo ja muutetaan se digitaaliseksi
}
