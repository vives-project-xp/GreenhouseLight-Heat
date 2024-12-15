#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFi.h>       // For ESP32
#include <FastLED.h>
#include <WiFiUdp.h>  // For UDP communication

// Wi-Fi credentials
const char* ssid = "telenet-9FB96";
const char* password = "Rtpkfe3Vb4yd";

// Web server
WebServer server(80);
WiFiUDP udp;

// Temperature and brightness values
int OpenShadeAtTemp = 30;  
int CloseShadeAtTemp = 25;
int minValueIdealBrightness = 200;
int maxValueIdealBrightness = 700;
int TurnOnHeater = 20;
int TurnOffHeater = 25;

#define STEP_PIN 4  // GPIO for STEP+
#define DIR_PIN 5   // GPIO for DIR+

// WiZ smartplug
const char* deviceIP = "10.10.2.77";
const int udpPort = 38899;

long long temperature = -1;
long long brightnessSensor = 500;
int brightnessLEDS = 0;
bool ShadeIsOpen = false;

#define NUM_LEDS 144
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);

  // Setup stepper pins
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  WiFi.disconnect(true);
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

  // Configure server routes
  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/json/state", HTTP_POST, handleSensorData);

  server.begin();
  Serial.println("HTTP server started");

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setRgbw(RgbwDefault());

  udp.begin(udpPort);
  Serial.println("UDP Initialized");

  // Set up initial LED state
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 0);  // Purple
  }
  FastLED.show();
}

void handleRoot() {
  char buffer[2048];
  snprintf(buffer, sizeof(buffer), R"rawliteral(
    <h1>Pas de waarden aan</h1>
    <form action="/save" method="POST">
        <label for="OpenShadeAtTemp">Open Shade At Temp(°C):</label>
        <input type="number" id="OpenShadeAtTemp" name="OpenShadeAtTemp" value="%d"><br>

        <label for="CloseShadeAtTemp">Close Shade At Temp(°C):</label>
        <input type="number" id="CloseShadeAtTemp" name="CloseShadeAtTemp" value="%d"><br>

        <label for="minValueIdealBrightness">Min Value Ideal Brightness:</label>
        <input type="number" id="minValueIdealBrightness" name="minValueIdealBrightness" value="%d"><br>

        <label for="maxValueIdealBrightness">Max Value Ideal Brightness:</label>
        <input type="number" id="maxValueIdealBrightness" name="maxValueIdealBrightness" value="%d"><br>

        <label for="TurnOnHeater">Turn On Heater(°C):</label>
        <input type="number" id="TurnOnHeater" name="TurnOnHeater" value="%d"><br>

        <label for="TurnOffHeater">Turn Off Heater(°C):</label>
        <input type="number" id="TurnOffHeater" name="TurnOffHeater" value="%d"><br>

        <button type="submit">Opslaan</button>
    </form>
  )rawliteral", OpenShadeAtTemp, CloseShadeAtTemp, minValueIdealBrightness, maxValueIdealBrightness, TurnOnHeater, TurnOffHeater);

  server.send(200, "text/html", buffer);
}

void handleSave() {
  if (server.hasArg("OpenShadeAtTemp")) {
    OpenShadeAtTemp = server.arg("OpenShadeAtTemp").toInt();
  }
  if (server.hasArg("CloseShadeAtTemp")) {
    CloseShadeAtTemp = server.arg("CloseShadeAtTemp").toInt();
  }
  if (server.hasArg("minValueIdealBrightness")) {
    minValueIdealBrightness = server.arg("minValueIdealBrightness").toInt();
  }
  if (server.hasArg("maxValueIdealBrightness")) {
    maxValueIdealBrightness = server.arg("maxValueIdealBrightness").toInt();
  }
  if (server.hasArg("TurnOnHeater")) {
    TurnOnHeater = server.arg("TurnOnHeater").toInt();
  }
  if (server.hasArg("TurnOffHeater")) {
    TurnOffHeater = server.arg("TurnOffHeater").toInt();
  }

  Serial.println("Nieuwe waarden ontvangen:");
  Serial.print("OpenShadeAtTemp: "); Serial.println(OpenShadeAtTemp);
  Serial.print("CloseShadeAtTemp: "); Serial.println(CloseShadeAtTemp);
  Serial.print("minValueIdealBrightness: "); Serial.println(minValueIdealBrightness);
  Serial.print("maxValueIdealBrightness: "); Serial.println(maxValueIdealBrightness);
  Serial.print("TurnOnHeater: "); Serial.println(TurnOnHeater);
  Serial.print("TurnOffHeater: "); Serial.println(TurnOffHeater);

  server.send(200, "text/html", "<h1>Instellingen opgeslagen!</h1><a href='/'>Ga terug</a>");
}

void handleSensorData() {
  StaticJsonDocument<200> doc;

  if (server.hasArg("plain")) {
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    if (doc.containsKey("temperature")) {
      temperature = doc["temperature"];
    }
    if (doc.containsKey("brightness")) {
      brightnessSensor = doc["brightness"];
    }

    Serial.print("Received Temperature: ");
    Serial.println(temperature);
    Serial.print("Received Brightness: ");
    Serial.println(brightnessSensor);

    fixLight();
    fixHeat();

    server.send(200, "application/json", "{\"status\":\"Data received\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}

void fixHeat() {
  if (temperature < TurnOnHeater) {
    Serial.println("Temperature below 20°C: Turning smart plug ON");
    sendWiZCommand(true);
  } else if (temperature > TurnOffHeater) {
    Serial.println("Temperature 20°C or above: Turning smart plug OFF");
    sendWiZCommand(false);
  }
  if (temperature > OpenShadeAtTemp && !ShadeIsOpen) {
    OpenShade();
  } else if (temperature < CloseShadeAtTemp && ShadeIsOpen) {
    CloseShade();
  }
}

void fixLight() {
   if (brightnessSensor < minValueIdealBrightness) {
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
    else if (brightnessSensor > maxValueIdealBrightness) {
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
}

void sendWiZCommand(bool state) {
  String command = String("{\"method\":\"setState\",\"params\":{\"state\":") + (state ? "true" : "false") + String("}}");

  udp.beginPacket(deviceIP, udpPort);
  udp.print(command);
  udp.endPacket();

  Serial.print("WiZ Command sent: ");
  Serial.println(command);
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

void loop() {
  server.handleClient();
  delay(2000);
}
