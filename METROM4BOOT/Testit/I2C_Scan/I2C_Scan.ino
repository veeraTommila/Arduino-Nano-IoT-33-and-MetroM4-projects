// --------------------------------------
// i2c_scanner
//
// Modified from https://playground.arduino.cc/Main/I2cScanner/
// --------------------------------------

#include <Wire.h>

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire

void setup() {
  WIRE.begin();

  Serial.begin(9600);
  Serial1.begin(115200);
  while (!Serial)
     delay(10);
  Serial.println("\nI2C Scanner");
}


void loop() {
  byte error, address;
  int nDevices; //Laitteiden lukumäärä.

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )   // Käydään laitteiden osoitteet läpi desimaalein 1 - 127.
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    WIRE.beginTransmission(address);  //Testataan, löytyykö osoite.
    error = WIRE.endTransmission(); //Jos osoite ei löydy, ei oteta sitä laskuihin mukaan.

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);  //Jos osoite löytyy, tulostetaan sen tiedot.
      Serial.println("  !");

      nDevices++; //Kasvatetaan laskuriarvoa yhdellä.
    }
    else if (error==4) //Jos mitään ei löydy, tehdään tämä.
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
  }