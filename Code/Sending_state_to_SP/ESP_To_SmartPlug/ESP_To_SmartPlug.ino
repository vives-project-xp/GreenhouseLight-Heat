#include <WiFi.h>       // For ESP32; use <ESP8266WiFi.h> for ESP8266
#include <WiFiUdp.h>    // For UDP communication

const char* ssid = "devbit";           // Replace with your Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Replace with your Wi-Fi password

const char* deviceIP = "10.10.2.77";   // Replace with the IP address of your WiZ smart plug
const int udpPort = 38899;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Initialize UDP
  udp.begin(udpPort);
  Serial.println("UDP Initialized");
}

void loop() {
  // Example: Turn the smart plug on
  sendWiZCommand(true);
  delay(5000); // Wait 5 seconds

  // Example: Turn the smart plug off
  sendWiZCommand(false);
  delay(5000); // Wait 5 seconds
}

void sendWiZCommand(bool state) {
  // Create JSON command
  String command = String("{\"method\":\"setState\",\"params\":{\"state\":") +
                   (state ? "true" : "false") +
                   String("}}");

  // Send the command to the WiZ plug
  udp.beginPacket(deviceIP, udpPort);
  udp.print(command);
  udp.endPacket();

  Serial.print("Command sent: ");
  Serial.println(command);
}
