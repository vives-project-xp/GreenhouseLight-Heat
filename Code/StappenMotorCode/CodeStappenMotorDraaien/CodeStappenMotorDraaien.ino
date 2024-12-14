#define STEP_PIN 4  // GPIO4 voor PUL+
#define DIR_PIN 5    // GPIO5 voor DIR+

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
}

void loop() {
  digitalWrite(DIR_PIN, LOW);  // Draairichting vooruit instellen

  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delay(10);  // Snelheid verhogen, kleiner delay
    digitalWrite(STEP_PIN, LOW);
    delay(10);
  }

  delay(2000);  // Pauze van 2 seconden

    digitalWrite(DIR_PIN, HIGH);  // Draairichting vooruit instellen

  for (int i = 0; i < 200; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delay(10);  // Snelheid verhogen, kleiner delay
    digitalWrite(STEP_PIN, LOW);
    delay(10);
  }

  delay(2000);
}
