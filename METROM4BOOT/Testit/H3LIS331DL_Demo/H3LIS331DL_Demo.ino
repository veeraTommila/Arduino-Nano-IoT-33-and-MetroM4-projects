/* get accelerate data of H3LIS331DL
 * Auth : lawliet(lawliet.zou@gmail.com)
 * version : 0.1
 Seeed Studio library
 */
// Kiihtyvyysanturimittauksia. Kiihtyvyysanturia voidaan käyttää hälyttimenä, kun mitataan esim. oven avaamista.
#include <H3LIS331DL.h>
#include <Wire.h>

//please get these value by running H3LIS331DL_AdjVal Sketch.
#define VAL_X_AXIS  20 //203
#define VAL_Y_AXIS  27 //165
#define VAL_Z_AXIS  13 //371

H3LIS331DL h3lis;

void setup(){
  Serial.begin(9600);
  h3lis.init();
  h3lis.importPara(VAL_X_AXIS,VAL_Y_AXIS,VAL_Z_AXIS);
}

void loop(){
  /*int16_t x,y,z;
  h3lis.readXYZ(&x,&y,&z);
  Serial.print("x, y, z = ");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.println(z);*/

  double xyz[3];
  double gee;
  h3lis.getAcceleration(xyz); //Luetaan XYZ-arvoja ja talletetaan ne taulukkoon.
  gee = sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);  // Summakiihtyvyys = laskettu arvo.
  if(gee<1){
    Serial.println(0);  // Jos tunnistetaan <1 suuruisia kiihtyvyysarvoja. Tulostetaan testissä nolla-arvot.
  }
  else{
    Serial.println(gee); // Jos tunnistetaan >1 suuruisia kiihtyvyysarvoja. Tulostetaan laskettu arvo.

  }
  
  delay(100);
}
