#include <ArduinoJson.h>
#include <WiFi.h>  // Voor ESP32
#include <FastLED.h>
#include <WiFiUdp.h>  // Voor UDP communicatie
#include <WebServer.h>

// Wi-Fi-instellingen
const char* ssid = "devbit";       // Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Wi-Fi wachtwoord

const char* deviceIP = "10.10.2.77";   // Replace with the IP address of your WiZ smart plug
const int udpPort = 38899;

WiFiUDP udp;  // Create an instance of WiFiUDP
WebServer server(80);  // Initialiseer webserver op poort 80

#define STEP_PIN 4  // GPIO4 voor PUL+
#define DIR_PIN 5    // GPIO5 voor DIR+

// Number of LEDs in your strip
#define NUM_LEDS 144

// Pin for controlling the LEDs
#define DATA_PIN 2

// Define the array of LEDs
CRGB leds[NUM_LEDS];

// Time scaling factors for each component (optional, can be removed if not used)
#define TIME_FACTOR_HUE 60
#define TIME_FACTOR_SAT 100
#define TIME_FACTOR_VAL 100

// Variabelen om ontvangen gegevens op te slaan
long long temperature = -1; // Begin met een ongeldige waarde
long long brightness = -1;  // Begin met een ongeldige waarde

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

    // Stuur een bevestiging terug naar de client
    server.send(200, "application/json", "{\"status\":\"Data received\"}");

    // Temperature control
    if (temperature < 20) {
      sendWiZCommand(true);  // Turn on the smart plug if temperature is below 20
    } else if (temperature > 25) {
      sendWiZCommand(false);  // Turn off the smart plug if temperature is above 25
    }

    // Motor control for shades
    if (temperature > 24) {
      lowerShades();  // Lower the shades if temperature is above 24
    } else if (temperature < 30) {
      openShades();  // Open the shades if temperature is below 30
    }

    // Brightness control
    if (brightness < 100) {
      FastLED.setBrightness(255);  // Turn on the lamp at full brightness
    } else if (brightness > 500) {
      FastLED.setBrightness(0);  // Turn off the lamp
    } else {
      int adjustedBrightness = map(brightness, 100, 500, 255, 0);
      FastLED.setBrightness(adjustedBrightness);  // Adjust brightness proportionally
    }
    FastLED.show();  // Update the LEDs
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}

void setup() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

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

  // Initialize UDP
  udp.begin(udpPort);
  Serial.println("UDP Initialized");

  // Initialize LEDs with RGBW configuration
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());
  FastLED.setBrightness(255);  // Set global brightness to 50%

  // Set all LEDs to purple (mix of red and blue)
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 255, 0); // Rood , Blauw , Groen
  }
  FastLED.show();  // Update the LEDs
}

void sendWiZCommand(bool state) {
  udp.beginPacket(deviceIP, udpPort);
  StaticJsonDocument<200> doc;
  doc["method"] = "setPilot";
  JsonObject params = doc.createNestedObject("params");
  params["state"] = state;
  char buffer[256];
  size_t n = serializeJson(doc, buffer);
  udp.write((const uint8_t*)buffer, n);  // Cast buffer to const uint8_t*
  udp.endPacket();
}

void lowerShades() {
  digitalWrite(DIR_PIN, LOW);  // Draairichting vooruit instellen

  for (int i = 0; i < 2400; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);  // Adjust speed by changing delay
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void openShades() {
  digitalWrite(DIR_PIN, HIGH);  // Draairichting achteruit instellen

  for (int i = 0; i < 2400; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);  // Adjust speed by changing delay
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
}

void loop() {
  // Verwerk binnenkomende HTTP-verzoeken
  server.handleClient();
}