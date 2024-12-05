#include <wled.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "devbit";          // Your Wi-Fi SSID
const char* password = "Dr@@dloos!";  // Your Wi-Fi password

// Create an instance of the server
AsyncWebServer server(80);

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println("ESP32 IP address: " + WiFi.localIP().toString());

  // Define POST route for motor control
  server.on("/motor", HTTP_POST, [](AsyncWebServerRequest *request) {
    // Check if the request has "steps" parameter
    if (request->hasParam("steps", true)) {
      String steps = request->getParam("steps", true)->value();
      Serial.println("Received steps: " + steps);

      // Send response back to client
      request->send(200, "application/json", "{\"status\": \"Value received\", \"steps\": " + steps + "}");
    } else {
      Serial.println("Error: No 'steps' parameter found");
      request->send(400, "application/json", "{\"error\": \"Missing 'steps' parameter\"}");
    }
  });

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // No need for logic here; server handles everything asynchronously
}
