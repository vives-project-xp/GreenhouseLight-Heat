#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h> // Voor ESP32
#include <FastLED.h>

// Wi-Fi-instellingen
const char* ssid = "devbit";       // Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Wi-Fi wachtwoord

WebServer server(80);  // Initialiseer webserver op poort 80

// Variabelen om ontvangen gegevens op te slaan
long long temperature; // Begin met een ongeldige waarde
long long brightness;  // Begin met een ongeldige waarde

// LED-instellingen
#define NUM_LEDS 144
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

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
      brightness = doc["brightness"].as<long long>();
    }

    // Print de ontvangen waarden naar de seriële monitor
    Serial.print("Received Temperature: ");
    Serial.println(temperature);
    Serial.print("Received Brightness: ");
    Serial.println(brightness);

    // Update de helderheid van de LED-strip
    FastLED.setBrightness(brightness); // Pas de helderheid van de LED-strip aan

    // Stuur een bevestiging terug naar de client
    server.send(200, "application/json", "{\"status\":\"Data received\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}

void setup() {
  Serial.begin(115200);

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

  // Initialize LEDs with RGBW configuration
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());
}

void loop() {
  // Verwerk binnenkomende HTTP-verzoeken
  server.handleClient();

  // Stel de LED-strip in op paars
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 0); // Paars
  }
  FastLED.show();  // Update de LEDs

  // Print de waarden regelmatig naar de seriële monitor (voor debuggen)
  Serial.print("Current Temperature: ");
  Serial.println(temperature);
  Serial.print("Current Brightness: ");
  Serial.println(brightness);
}
