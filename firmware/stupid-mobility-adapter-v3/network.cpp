#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <SPIFFS.h>
//#include "OTA.h"

// Replace with your network credentials
//const char* ssid = "REPLACE_WITH_YOUR_SSID";
//const char* password = "REPLACE_WITH_YOUR_PASSWORD";
const char* ssid = "oort";
const char* password = "heliopause";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Stupid Mobility Adapter v3</title>
//  <meta name="viewport" content="width=device-width, initial-scale=1">
//  <link rel="icon" href="data:,">
</head>
<body>
  <h2>Stupid Mobility Adapter v3</h2>
</body>
</html>
)rawliteral";

void startNetwork(){
  //setupOTA("TemplateSketch", ssid, password);
    // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

//void doNetworking(){
//  //ArduinoOTA.handle();
//}
