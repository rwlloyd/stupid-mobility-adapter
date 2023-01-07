#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <WebSocketsServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <arduino.h>
#include "config.h"
#include "safety.h"
//#include "OTA.h"

// Replace with your network credentials
extern const char* hostname = "ESP32 ARWACv4";
extern const char* ssid = "ARWACv4";
extern const char* password = NULL;

const unsigned long updateInterval = 1000; //Basic update interval for webpage data
unsigned long previousMillis = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Json stuff for sending messages to websockets for UI/debugging
StaticJsonDocument<200> doc_tx;
StaticJsonDocument<200> doc_rx;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Stupid Mobility Adapter v3</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">

  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onopen = function(e) {console.log("[socket] socket.onopen "); };
      Socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
      Socket.onmessage = function(event) {  
        processCommand(event);
      };
    }

    // Function to process commands recieved by the client from the ucontroller
    function processCommand(event){
      var obj = JSON.parse(event.data);
      document.getElementById('estop').innerHTML = obj.estop;
      document.getElementById('enable').innerHTML = obj.enable;
      document.getElementById('servo').innerHTML = obj.servo;
      document.getElementById('battv').innerHTML = obj.battv;
      document.getElementById('throttle').innerHTML = obj.throttle;
      document.getElementById('steer').innerHTML = obj.steer;
      document.getElementById('tool').innerHTML = obj.tool;
      console.log(obj);
      console.log("[socket] " + event.data);
      //document.getElementById("my_value").innerHTML = obj.value;
    }

    window.onload = function(event){
      init();
    }

//    function sendText(){
//      Socket.send(document.getElementById("txBar").value);
//      document.getElementById("txBar").value = "";
//    }
//    function sendBrightness(){
//      Socket.send("#"+document.getElementById("brightness").value);
//    } 
   
  </script>
</head>
<body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center; padding-top:50px">
  <h2>Stupid Mobility Adapter v3</h2>
  <div> ESTOP: <span id='estop'>-</span></div>
  <div> Master Enable: <span id='enable'>-</span></div>
  <div> Servo In: <span id='servo'>-</span></div>
  <div> Battery Voltage: <span id='battv'>-</span> V</div>
  <div> Throttle: <span id='throttle'>-</span></div>
  <div> Steering: <span id='steer'>-</span></div>
  <div> Tool: <span id='tool'>-</span></div>
</body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] got Text: %s\n", num, payload);
       // try to decipher the JSON string received
      DeserializationError error = deserializeJson(doc_rx, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      break;
      // else {

      // }
  }
}

// response for 404 error
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void initWifi(){
  // very useful https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
  if (WIFI_TYPE == 1){
    // For just connecting to a known hotspot
    WiFi.mode(WIFI_STA);                      // instantiate wifi in station mode
    WiFi.begin(ssid, password);               // make sure this is set in the config when you set the WIFI_TYPE
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");                      // Wait for wifi connection not sure how it would timeout...
    }
  } else if (WIFI_TYPE == 2) {
    // For rocking your own hotspot because rural
    int channel = 6;        // Wi-Fi channel number (1-13)
    int ssid_hidden = 0;    // (0 = broadcast SSID, 1 = hide SSID)
    int max_connection = 4; //maximum simultaneous connected clients (1-4)
   
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password, channel, ssid_hidden, max_connection);
  } else if (WIFI_TYPE == 3){
    WiFi.mode(WIFI_AP_STA); 
  } else { // Just turn wifi off
    Serial.println("Continuing without configuring wifi");
  }
}

void startNetwork(){

  // Setup OTA updates
  //setupOTA("TemplateSketch", ssid, password); // Not currently used due to sketch size restrictions

  // setup Wi-Fi via function call ^
  initWifi();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send_P(200, "text/html", index_html);
  });

  // Route for 404 error
  server.onNotFound(notFound);

  // begin the http server
  server.begin();

  // begin the websocket server
  webSocket.begin();

  // setup the callback for webSocket events 
  webSocket.onEvent(webSocketEvent);
}

void doNetworking(){
  // spin the websocket
  webSocket.loop();

  // do we need to send a message...
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval){
    // Make and send a json object
    String jsonString="";
    JsonObject object = doc_tx.to<JsonObject>();
    object["estop"] = error;
    object["enable"] = enable;
    object["servo"] = enableServoInput;
    object["throttle"] = motorSpeed;
    object["steer"] = actuatorSetpoint_request;
    object["battv"] = batteryVoltage;
    object["tool"] = toolSetpoint_request;
    serializeJson(doc_tx, jsonString);
    webSocket.broadcastTXT(jsonString);
    Serial.println(jsonString);
    previousMillis = currentMillis;
    
  }
  //ArduinoOTA.handle();
}


      // This next line is copied from the last time i did something like this...
// else {
//}
//        // JSON string was received correctly, so information can be retrieved:
//        const char* msg_type = doc_rx["type"];
//        req_hyp = doc_rx["hyp"];
//        req_phx = doc_rx["phx"];
//        req_aux = doc_rx["aux"];
//        req_rex = doc_rx["rex"];
//
//        //      uint8_t command = (uint8_t) strtol((const char *) &payload[0], NULL, 16);
//       if(String(msg_type) == "request"){
//         Serial.printf("Got Request hyp:%d, phx:%d, aux:%d, rex:%d\n", req_hyp, req_phx, req_aux, req_rex);
////         if (req_hyp == 1){
////            webSocket.sendTXT(num, "HyperSpectral Triggered");
////          } else if (req_phx == 1){
////            webSocket.sendTXT(num, "Phenospex Triggered");
////          } else if (req_aux == 1){
////            webSocket.sendTXT(num, "Auxiliary Triggered");
////          }
//        }
//      }
      
      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      
      
//    case WStype_BIN:
//      Serial.printf("[%u] get binary length: %u\n", num, length);
////      hexdump(payload, length);
//      // send message to client
//      // webSocket.sendBIN(num, payload, length);
//      break;
//  }
//}
