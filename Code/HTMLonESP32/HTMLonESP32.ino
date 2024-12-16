#include <WebServer.h>
#include <WiFi.h>

// Wi-Fi instellingen
const char* ssid = "devbit";       // Wi-Fi SSID
const char* password = "Dr@@dloos!";   // Wi-Fi wachtwoord

// Webserver object
WebServer server(80);

// Variabelen die kunnen worden aangepast via de webinterface
int OpenShadeAtTemp = 30;
int CloseShadeAtTemp = 25;
int minValueIdealBrightness = 200;
int maxValueIdealBrightness = 700;
int TurnOnHeater = 20;
int TurnOffHeater = 25;

// HTML-pagina om waarden aan te passen
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Instellingen aanpassen</title>
    <meta charset="UTF-8">
</head>
<body>
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
</body>
</html>
)rawliteral";


void handleRoot() {
    char buffer[2048];
    snprintf(buffer, sizeof(buffer), htmlPage, OpenShadeAtTemp, CloseShadeAtTemp, minValueIdealBrightness, maxValueIdealBrightness, TurnOnHeater, TurnOffHeater);
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

    // Print de nieuwe waarden naar de seriële monitor
    Serial.println("Nieuwe waarden ontvangen:");
    Serial.print("OpenShadeAtTemp: "); Serial.println(OpenShadeAtTemp);
    Serial.print("CloseShadeAtTemp: "); Serial.println(CloseShadeAtTemp);
    Serial.print("minValueIdealBrightness: "); Serial.println(minValueIdealBrightness);
    Serial.print("maxValueIdealBrightness: "); Serial.println(maxValueIdealBrightness);
    Serial.print("TurnOnHeater: "); Serial.println(TurnOnHeater);
    Serial.print("TurnOffHeater: "); Serial.println(TurnOffHeater);

    // Bevestiging terugsturen naar de gebruiker
    server.send(200, "text/html", "<h1>Instellingen opgeslagen!</h1><a href='/'>Ga terug</a>");
}

void setup() {
    Serial.begin(115200);

    // Verbinden met Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configureer serverroutes
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);

    // Start de webserver
    server.begin();
    Serial.println("HTTP server gestart");
}

void loop() {
    server.handleClient();
}
