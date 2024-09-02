// Veera Tommila. veera.tommila@gmail.com
/* Exercise 3 about interuptions.
When button B1 is pressed, an interuption occurs and temperature and humidity are measured with the SHTC3 sensor and the following is printed on the monitor:
The temperature is xx.x °C and the relative humidity is yy.y %
*/
#include "Adafruit_SHTC3.h"
#include "Adafruit_VL53L0X.h"

const int PwrSht = 12;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();  // Object variable for SHT to refer to the sensor.
Adafruit_VL53L0X lox = Adafruit_VL53L0X();  // Lox is an entity that refers to the sensor.

int ledRed = 7; // LED connected to digital pin 7.
int Button2 = 3;    // pushbutton (button B1) connected to digital pin 3.
int buttonPushCounter = 0; // Used in the switch statement below.

long time = 0; // Time since last action occured.
long debounce = 200; // Debounce time to increase if the output is flickering.

volatile byte state_Button2 = LOW;  // There should be a state data variable for the button, which is 0 at startup.
volatile byte state_red = LOW;  // State data variable for the red LED, which is 0 at startup.

void setup() {
  pinMode(PwrSht, OUTPUT);
  pinMode(ledRed, OUTPUT);  // Sets the digital pin 7 as output
  digitalWrite(PwrSht, HIGH);
  Serial.begin(115200);

  while (!Serial)
    delay(1);     // Will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHTC3 testaus ja Adafruit VL53L0X testaus.");
  if (! shtc3.begin()) {  // If the sensor is not found, let's get stuck here.
    Serial.println("Ei löytynyt SHTC3-anturia.");
    while (1) delay(1);
  }
  if (!lox.begin()) {
    Serial.println(F("Ei voitu käynnistää VL53L0X-anturia"));
    while(1);
  }
  
  Serial.println("Löydettiin SHTC3-anturi"); // If the start-up is successful, it is reported that the sensor has been found.    
  pinMode(Button2, INPUT_PULLUP);    // Sets the digital pin 3 as input
// power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
 //attachInterrupt(digitalPinToInterrupt(ledRed), interuption, CHANGE);  // Interrupt assignment for pushbutton 1, which as an IO input causes an interrupt. Let's call the program alarm. Where does the interruption occur (falling, rising or change).
// start continuous ranging
  lox.startRangeContinuous();  
  attachInterrupt(digitalPinToInterrupt(Button2), interuption, FALLING); // Interrupt assignment for pushbutton 2, which as an IO input causes an interrupt. Let's call the program interrupt. Where does the interruption occur (falling, rising or change).  
}

void loop() {
  digitalWrite(ledRed, state_red);
  digitalWrite(Button2, state_Button2);  
  //Serial.print("Distance mm: ");Serial.println(lox.readRange());   
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading measurements... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);    // Measure and print the value

//     
  if (measure.RangeMilliMeter >= 20 && measure.RangeMilliMeter <= 500) {
    Serial.print("You are too close!");
    state_red = !state_red;  
  } else {
    delay(500);
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);    // Measure and print the value
    state_red = state_red;         
  }  
}

void interuption(){
  state_Button2 = !state_Button2;  
  sensors_event_t humidity, temp;  
  shtc3.getEvent(&humidity, &temp); // Let's use this command to read humidity and temperature. 
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" °C.");  // Let's change Temperature to degrees Celsius.
  Serial.print("Relative humidity: "); Serial.print(humidity.relative_humidity); Serial.println("%."); // The relative humidity is measured.
}
/*
void alarm(){
  if(lox.readRange() < 50){
    Serial.print("Distancce mm: ");Serial.println(lox.readRange());
    Serial.println("You are too close!");
    state_red = !state_red; //From LOW to HIGH.
  }else
  {
    state_red = state_red;
  }
}*/