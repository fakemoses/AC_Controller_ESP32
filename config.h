#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <IRremoteESP8266.h>

struct Config {
  String ssid;
  String password;
  String acType;
  bool isConfigured;
};

extern Config config;

bool loadConfig();
bool saveConfig();
void resetConfig();

decode_type_t getProtocolFromString(const String &acType);
