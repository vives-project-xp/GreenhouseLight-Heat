#define PULSE_PIN 4 // GPIO4 voor PUL+
#define DIR_PIN 5   // GPIO5 voor DIR+

void setup() {
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  
}

void loop() {
  // Zet de draairichting
  digitalWrite(DIR_PIN, HIGH); // HIGH = vooruit, LOW = achteruit
  
  // Stuur pulsen naar de driver
  for (int i = 0; i < 200; i++) { // 200 stappen = 1 omwenteling bij 1,8° stappenmotor
    digitalWrite(PULSE_PIN, HIGH);
    delayMicroseconds(500); // Pulse-hoogte (500 µs)
    digitalWrite(PULSE_PIN, LOW);
    delayMicroseconds(500); // Pulse-laag (500 µs)
  }
  
  delay(1000); // Wacht 1 seconde voor de volgende beweging
}
