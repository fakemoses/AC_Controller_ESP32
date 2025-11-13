#pragma once
#include <ESPAsyncWebServer.h>
#include "IRac.h"

extern IRac ac;

const char control_html[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head>
<title>AC Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>button{width:80px;height:40px;}</style>
</head><body>
<h2>AC Control</h2>
<a href="/on"><button>ON</button></a>
<a href="/off"><button>OFF</button></a>
</body></html>)HTML";

void setupControlPage(AsyncWebServer &server) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", control_html);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    ac.next.power = true;
    ac.sendAc();
    request->send(200, "text/plain", "AC turned ON");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    ac.next.power = false;
    ac.sendAc();
    request->send(200, "text/plain", "AC turned OFF");
  });
}
