#include <ESP8266WiFi.h>  // or ESP8266WiFi.h
#include <ESPAsyncWebServer.h>
#include "IRremoteESP8266.h"
#include "IRac.h"
#include "config.h"
#include "setup_page.h"
#include "control_page.h"

AsyncWebServer server(80);
const int irPin = 4;  // Change to your actual IR LED pin
IRac ac(irPin);

void setup_ac() {
  ac.next.protocol = getProtocolFromString(config.acType);
  ac.next.model = 1;
  ac.next.mode = stdAc::opmode_t::kCool;
  ac.next.celsius = true;
  ac.next.degrees = 25;
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;
  ac.next.power = false;
  ac.next.swingv = stdAc::swingv_t::kOff;
  ac.next.swingh = stdAc::swingh_t::kOff;
  ac.next.light = false;
  ac.next.beep = false;
  ac.next.econo = false;
  ac.next.filter = false;
  ac.next.turbo = false;
  ac.next.quiet = false;
  ac.next.sleep = -1;
  ac.next.clean = false;
  ac.next.clock = -1;

  Serial.print("Using AC Protocol: ");
  Serial.println(config.acType);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!loadConfig() || !config.isConfigured) {
    Serial.println("No config found. Starting setup mode...");
    WiFi.softAP("ESP_AC_SETUP", "12345678");
    IPAddress IP = WiFi.softAPIP();
    Serial.println("Please connect to the following Wifi Network:");
    Serial.print("Wifi Name: ESP_AC_SETUP");
    Serial.print("12345678");
    Serial.print("AP IP for setup: "); Serial.println(IP);
    setupFirstTimePage(server);
  } else {
    Serial.println("Config found. Connecting...");
    WiFi.begin(config.ssid.c_str(), config.password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected! IP: " + WiFi.localIP().toString());

    setup_ac();
    setupControlPage(server);
  }

  server.begin();
}

void loop() {
  // Nothing — async handles everything
}
