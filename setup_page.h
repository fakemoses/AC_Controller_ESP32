#pragma once
#include <ESPAsyncWebServer.h>
#include "config.h"

const char setup_html[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head>
<title>AC Setup</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body { font-family: Arial; text-align: center; }
  select, input { width: 200px; margin: 5px; }
</style></head><body>
<h2>First Time Setup</h2>
<form action="/save">
  WiFi SSID: <input type="text" name="ssid"><br>
  Password: <input type="password" name="password"><br>
  AC Type:
  <select name="acType">
    <option value="PANASONIC_AC32">Panasonic</option>
    <option value="DAIKIN">Daikin</option>
    <option value="DAIKIN2">Daikin (Alt)</option>
    <option value="LG">LG</option>
    <option value="MITSUBISHI">Mitsubishi</option>
    <option value="HITACHI">Hitachi</option>
    <option value="SAMSUNG">Samsung</option>
  </select><br>
  <input type="submit" value="Save">
</form>
</body></html>)HTML";

void setupFirstTimePage(AsyncWebServer &server) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", setup_html); 
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("ssid") && request->hasParam("password") && request->hasParam("acType")) {
      config.ssid = request->getParam("ssid")->value();
      config.password = request->getParam("password")->value();
      config.acType = request->getParam("acType")->value();
      saveConfig();
      request->send(200, "text/html", "<h3>Saved! Rebooting...</h3>");
      delay(1000);
      ESP.restart();
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });
}
