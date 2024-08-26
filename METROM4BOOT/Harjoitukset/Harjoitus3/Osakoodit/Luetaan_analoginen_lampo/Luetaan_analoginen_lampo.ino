// Analogisen tiedon lukeminen ja muuttaminen digitaaliseksi. Tässä tapauksessa luetaan tietoa analogiselta lämpötila-anturilta.
int analogPin = A1; // Analogisen lämpötila-anturin NTC-liitäntä. 
int pwrPin = A2;  // Analogisen lämpötila-anturin PWR_NTC-liitäntä.
int val = 0;  // variable to store the value read

void setup() {
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, HIGH); // Pannaan virrat päälle PWR_NTC-liitäntään.
  Serial.begin(9600);         //  setup serial
}

void loop() {
  analogReadResolution(10); // Analogipinnien resoluutio bitteinä.
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);          // debug value
  delay(2000);
}