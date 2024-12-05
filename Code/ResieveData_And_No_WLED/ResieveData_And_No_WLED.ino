#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h> // Voor ESP32

// Wi-Fi-instellingen
const char* ssid = "devbit";       // Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Wi-Fi wachtwoord

WebServer server(80);  // Initialiseer webserver op poort 80

// Variabelen om ontvangen gegevens op te slaan
long long temperature; // Begin met een ongeldige waarde
long long brightness;  // Begin met een ongeldige waarde

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
}

void loop() {
  // Verwerk binnenkomende HTTP-verzoeken
  server.handleClient();

  // Print de waarden regelmatig naar de seriële monitor (voor debuggen)
  Serial.print("Current Temperature: ");
  Serial.println(temperature);
  Serial.print("Current Brightness: ");
  Serial.println(brightness);

  //delay(5000); // Pauzeer 5 seconden tussen prints
}
