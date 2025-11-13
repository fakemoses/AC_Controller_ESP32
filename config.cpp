#include "config.h"
#include <IRremoteESP8266.h>
#include <IRac.h>

decode_type_t getProtocolFromString(const String &acType) {
  if (acType.equalsIgnoreCase("PANASONIC_AC32")) return decode_type_t::PANASONIC_AC32;
  if (acType.equalsIgnoreCase("DAIKIN")) return decode_type_t::DAIKIN;
  if (acType.equalsIgnoreCase("DAIKIN2")) return decode_type_t::DAIKIN2;
  if (acType.equalsIgnoreCase("LG")) return decode_type_t::LG;
  if (acType.equalsIgnoreCase("MITSUBISHI")) return decode_type_t::MITSUBISHI_AC;
  if (acType.equalsIgnoreCase("HITACHI")) return decode_type_t::HITACHI_AC;
  if (acType.equalsIgnoreCase("SAMSUNG")) return decode_type_t::SAMSUNG_AC;
  return decode_type_t::UNKNOWN;
}

Config config;

bool loadConfig() {
  if (!LittleFS.begin()) return false;
  if (!LittleFS.exists("/config.json")) return false;

  File file = LittleFS.open("/config.json", "r");
  if (!file) return false;

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) return false;

  config.ssid = doc["ssid"].as<String>();
  config.password = doc["password"].as<String>();
  config.acType = doc["acType"].as<String>();
  config.isConfigured = doc["isConfigured"] | false;

  return true;
}

bool saveConfig() {
  if (!LittleFS.begin()) {
  Serial.println("Formatting LittleFS...");
  LittleFS.format();
  LittleFS.begin();
}

  StaticJsonDocument<256> doc;
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["acType"] = config.acType;
  doc["isConfigured"] = true;

  File file = LittleFS.open("/config.json", "w");
  if (!file) return false;
  serializeJson(doc, file);
  file.close();

  return true;
}

void resetConfig() {
  LittleFS.remove("/config.json");
}
