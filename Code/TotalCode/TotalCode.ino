#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h> // Voor ESP32
#include <FastLED.h>
#include <WiFiUdp.h> // Voor UDP communicatie

// Stepper instellen
#define STEP_PIN 4  // GPIO4 voor PUL+
#define DIR_PIN 5    // GPIO5 voor DIR+

// Wi-Fi-instellingen
const char* ssid = "devbit";       // Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Wi-Fi wachtwoord

WebServer server(80);  // Initialiseer webserver op poort 80
WiFiUDP udp;           // Voor communicatie met de smartplug

// WiZ smartplug-instellingen
const char* deviceIP = "10.10.2.77"; // Vervang met het IP-adres van je WiZ smartplug
const int udpPort = 38899;

// Variabelen om ontvangen gegevens op te slaan
long long temperature = -1; // Begin met een ongeldige waarde
long long brightnessSensor = 500; // Standaard helderheid
int brightnessLEDS = 0;

// LED-instellingen
#define NUM_LEDS 144
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

// Functie om de WiZ smartplug te bedienen
void sendWiZCommand(bool state) {
  // Maak JSON-opdrach

  String command = String("{\"method\":\"setState\",\"params\":{\"state\":") +
                   (state ? "true" : "false") +
                   String("}}");

  // Stuur de opdracht naar de WiZ smartplug
  udp.beginPacket(deviceIP, udpPort);
  udp.print(command);
  udp.endPacket();

  Serial.print("WiZ Command sent: ");
  Serial.println(command);
}

bool ShadeIsOpen = false;   // als true = schaduw is er over de serre

// Functie om ontvangen JSON-gegevens te verwerken
void handleSensorData() {
  StaticJsonDocument<200> doc;

  if (server.hasArg("plain")) {
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    // Update temperatuur en helderheid alleen bij succesvolle parsing
    if (doc.containsKey("temperature")) {
      temperature = doc["temperature"].as<long long>();
    }
    if (doc.containsKey("brightness")) {
      brightnessSensor = doc["brightness"].as<long long>();
    }

    // Print de ontvangen waarden naar de seriële monitor
    Serial.print("Received Temperature: ");
    Serial.println(temperature);
    Serial.print("Received Brightness: ");
    Serial.println(brightnessSensor);

  
    if (brightnessSensor < 270) {
      if (brightnessLEDS >= 235)
      {
        brightnessLEDS = 255;
      } else {
        brightnessLEDS += 10;
      }
      FastLED.setBrightness(brightnessLEDS);
      FastLED.show();
      Serial.print("LEDs more light +10 they are now:");
      Serial.println(brightnessLEDS);
    } 
    else if (brightnessSensor > 700) {
      if (brightnessLEDS <= 20)
      {
        brightnessLEDS = 0;
      } else {
        brightnessLEDS -= 10;
      }
      FastLED.setBrightness(brightnessLEDS);
      FastLED.show();
      Serial.print("LEDs less light (-10) they are now:");
      Serial.println(brightnessLEDS);
    }


    // Controleer of de temperatuur onder de 20 graden is
    if (temperature < 20) {
      Serial.println("Temperature below 20°C: Turning smart plug ON");
      sendWiZCommand(true); // Zet de smartplug aan
    } else if(temperature>25) {
      Serial.println("Temperature 20°C or above: Turning smart plug OFF");
      sendWiZCommand(false); // Zet de smartplug uit
    }


    if (temperature>30 && ShadeIsOpen == false) {
      OpenShade();
    } else if(temperature<25 && ShadeIsOpen == true){
      CloseShade();
    }

    // Stuur een bevestiging terug naar de client
    server.send(200, "application/json", "{\"status\":\"Data received\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}

void OpenShade(){
  ShadeIsOpen = true;

  digitalWrite(DIR_PIN, LOW);  // Draairichting vooruit instellen
  Serial.println("Going to OpenShade temp getting high");
  for (int i = 0; i < 2400; i++) { // 200 keer lus doorlopen is 360 graden draaien
    digitalWrite(STEP_PIN, HIGH); // omtrek buizen van ons 12.5 cm doek moet 1.6m uit rollen = ong 12 rotaties = 200*12 = 2400
    delayMicroseconds(1000);  // Snelheid verhogen, kleiner delay
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void CloseShade(){
  ShadeIsOpen = false;

  digitalWrite(DIR_PIN, HIGH);  // Draairichting vooruit instellen
  Serial.println("Going to CloseShade temp getting low");
  for (int i = 0; i < 2400; i++) { // 200 keer lus doorlopen is 360 graden draaien
    digitalWrite(STEP_PIN, HIGH); // omtrek buizen van ons 12.5 cm doek moet 1.6m uit rollen = ong 12 rotaties = 200*12 = 2400
    delayMicroseconds(1000);  // Snelheid verhogen, kleiner delay
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void setup() {
  Serial.begin(115200);
  
  // stepper als output zetten
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  // Verbinden met Wi-Fi
  WiFi.disconnect(true); // Zorg dat Wi-Fi losgekoppeld is voor een schone start
  delay(1000);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configureer de serverroute om JSON-gegevens te ontvangen
  server.on("/json/state", HTTP_POST, handleSensorData);

  // Start de webserver
  server.begin();
  Serial.println("HTTP server started");

  // Initialize LEDs met RGBW-configuratie
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());

  // volledige ledstrip op paars zetten
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 0); // Paars
  }
  FastLED.setBrightness(0);
  FastLED.show(); // Update de LEDs

  // Initializeer UDP voor de WiZ smartplug
  udp.begin(udpPort);
  Serial.println("UDP Initialized");
}

void loop() {
  // Verwerk binnenkomende HTTP-verzoeken
  server.handleClient();
  delay(2000); // iedere 5 sec binnen lezen enz
}
