
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
 
const char* ssid = "...";
const char* password =  "...";

const int PIN_ALPHA  = 0;
const int PIN_RED    = 1;
const int PIN_GREEN  = 2;
const int PIN_BLUE   = 3;

AsyncWebServer server(80);

//note: alpha=5 bytes, max=31
void rgbSet(byte alpha, byte red, byte green, byte blue) {
  sigmaDeltaWrite(PIN_ALPHA, alpha);
  sigmaDeltaWrite(PIN_RED,   red);
  sigmaDeltaWrite(PIN_GREEN, green);
  sigmaDeltaWrite(PIN_BLUE,  blue);
}

void setupSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void setupWifi() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
}

void setupHttpd() {
  server.on("/lights/1/mode", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("r", true)) {
      sigmaDeltaWrite(PIN_RED, (request->getParam("r", true)->value()).toInt());
    }
    if (request->hasParam("g", true)) {
      sigmaDeltaWrite(PIN_GREEN, (request->getParam("g", true)->value()).toInt());
    }
    if (request->hasParam("b", true)) {
      sigmaDeltaWrite(PIN_BLUE, (request->getParam("b", true)->value()).toInt());
    }
    if (request->hasParam("a", true)) {
      sigmaDeltaWrite(PIN_ALPHA, (request->getParam("a", true)->value()).toInt());
    }
    
    request->send(200, "text/plain", "");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET && SPIFFS.exists(request->url())) {
      request->send(SPIFFS, request->url(), "text/html");
    } else {
      request->send(404, "text/plain", request->url());
    }
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
 
  server.begin();
}

static uint8_t i = 0;

void setup() {
  setupSPIFFS();
  setupWifi();
  setupHttpd();

  setupPinToChannel(16, PIN_ALPHA);
  setupPinToChannel(17, PIN_RED);
  setupPinToChannel(21, PIN_GREEN);
  setupPinToChannel(22, PIN_BLUE);
}

void setupPinToChannel(int pin, int channel) {
  sigmaDeltaSetup(channel, 490);
  sigmaDeltaAttachPin(pin, channel);
  sigmaDeltaWrite(channel, 255);
}

void loop()
{
  delay(25);
}
