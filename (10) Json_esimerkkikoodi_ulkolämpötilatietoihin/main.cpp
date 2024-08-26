// This is a Json code example to retrieve the information of outside temperature.
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFiNINA.h>

void printWifiStatus();
void haeJson();

float jsonTuulenNopeus;
float jsonTuulenSuunta;
int jsonTimeZone;
const char* jsonKaupunki; 
int status;

char ssid[] = "ChydeniusOPEN";        // Your network SSID (name)
char pass[] = "";    // Your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // Your network key index number (needed only for WEP)

// The server and the client of Json data.
char jsonServer[] = "api.openweathermap.org";
WiFiClient jsonClient;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only.
  }

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  // You're connected now, so print out the status:
  printWifiStatus();
}

  void loop(){
    haeJson();
    delay(30000);
  }

  void printWifiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// In the subroutine haeJson, the Json record is retrieved into the variable.
void haeJson(){
 Serial.println("\nStarting connection to JSONserver...");
  // If you get a connection, report back via serial:
  if (jsonClient.connect(jsonServer, 80)) {
    Serial.println("connected to server");
    
    // Send HTTP request:
    jsonClient.println(F("GET /data/2.5/weather?q=Kokkola&appid=aa65788a0c7617082906de01f740ad35&units=metric HTTP/1.0")); 
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

    // Parse JSON object
    DeserializationError error = deserializeJson(doc, jsonClient);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      jsonClient.stop();
      return;
    }

    // Extract values
    jsonTuulenNopeus = doc["wind"]["speed"];
    jsonTuulenSuunta = doc["main"]["deg"];
    jsonTimeZone = doc["timezone"];
    jsonKaupunki = doc["name"];
    Serial.println();
    Serial.println("Haun tulokset:");
    Serial.print("Tuulen nopeus: ");
    Serial.println(jsonTuulenNopeus);
    Serial.print("Tuulen suunta: ");
    Serial.println(jsonTuulenSuunta);
    Serial.print("Aikavyöhyke: ");
    Serial.println(jsonTimeZone);
    Serial.print("Kaupunki: ");
    Serial.println(jsonKaupunki);

    // Disconnect
    jsonClient.stop();
      
  }
}
