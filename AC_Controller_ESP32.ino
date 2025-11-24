#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "IRremoteESP8266.h"
#include "IRac.h"
#include "config.h"
#include "setup_page.h"
#include "control_page.h"

//// TODO
//// LittleFS writes one time. How about next compile? address this
//// Allow user to change info, connection, AC type. 
//// Give more robust info to the user

AsyncWebServer server(80);
const int irPin = 4;
IRac ac(irPin);

bool triggeredToday[10] = {false};

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
  checkSchedules();
}

void checkSchedules() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int hour   = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;

  for (int i = 0; i < scheduleConfig.count; i++) {
    ScheduleItem &s = scheduleConfig.items[i];

    if (!s.active) continue;

    if (hour == s.hour &&
        minute == s.minute &&
        !triggeredToday[i]) {

      if (s.turnOn) {
        ac.next.power = true;
        ac.sendAc();
      } else {
        ac.next.power = false;
        ac.sendAc();
      }

      triggeredToday[i] = true;
    }
  }

  if (hour == 0 && minute == 0) {
    memset(triggeredToday, 0, sizeof(triggeredToday));
  }
}
