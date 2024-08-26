#include <Arduino.h>
#include <Arduino_LSM6DS3.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  
}
 
void loop() {
  float x, y, z, g; //x, y ja z ovat ko. suuntaisia kiihtyvyysarvoja ja g on niiden yhteisvaikutus

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);  // luetaan sensorin eri akseleiden kiihtyvyysarvot

    g = sqrt(x*x+y*y+z*z);  //lasketaan kiihtyvyyksien resultantti (Pythagoran lause)

    if(g>1.1){    //jos resultantti on yli 1.1 niin sen arvo tulostetaan 
    Serial.println(g);
    }
    else{
      Serial.println(0);  //muuten tulostetaan 0
    }

  }
}
