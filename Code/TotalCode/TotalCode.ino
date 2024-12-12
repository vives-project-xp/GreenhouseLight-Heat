#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h> // Voor ESP32
#include <FastLED.h>
#include <WiFiUdp.h> // Voor UDP communicatie

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
long long brightness = 128; // Standaard helderheid

// LED-instellingen
#define NUM_LEDS 144
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

// Functie om de WiZ smartplug te bedienen
void sendWiZCommand(bool state) {
  // Maak JSON-opdracht
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



bool ledsOn = false;

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

  
    if (!ledsOn && brightness < 270) {
        ledsOn = true;  // Zet de LED's aan
        FastLED.setBrightness(255);
        Serial.println("LEDs turned ON");
    } 
    else if (ledsOn && brightness < 220) {
        ledsOn = false;  // Zet de LED's uit
        FastLED.setBrightness(0);
        Serial.println("LEDs turned OFF");
    }



    // Controleer of de temperatuur onder de 20 graden is
    if (temperature < 20) {
      Serial.println("Temperature below 20°C: Turning smart plug ON");
      sendWiZCommand(true); // Zet de smartplug aan
    } else {
      Serial.println("Temperature 20°C or above: Turning smart plug OFF");
      sendWiZCommand(false); // Zet de smartplug uit
    }

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

  // Initialize LEDs met RGBW-configuratie
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());

  // Initializeer UDP voor de WiZ smartplug
  udp.begin(udpPort);
  Serial.println("UDP Initialized");
}

void loop() {
  // Verwerk binnenkomende HTTP-verzoeken
  server.handleClient();

  // Stel de LED-strip in op paars
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 0); // Paars
  }
  FastLED.show(); // Update de LEDs

  // Print de waarden regelmatig naar de seriële monitor (voor debuggen)
  Serial.print("Current Temperature: ");
  Serial.println(temperature);
  Serial.print("Current Brightness: ");
  Serial.println(brightness);

  delay(5000); // Een kleine vertraging voor stabiliteit
}
