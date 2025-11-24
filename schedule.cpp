#include <LittleFS.h>
#include "schedule.h"

ScheduleConfig scheduleConfig;

void saveSchedules() {
  DynamicJsonDocument doc(1024);

  JsonArray arr = doc.createNestedArray("schedules");
  for (int i = 0; i < scheduleConfig.count; i++) {
    JsonObject s = arr.createNestedObject();
    s["hour"]   = scheduleConfig.items[i].hour;
    s["minute"] = scheduleConfig.items[i].minute;
    s["turnOn"] = scheduleConfig.items[i].turnOn;
    s["active"] = scheduleConfig.items[i].active;
  }

  File f = LittleFS.open("/schedules.json", "w");
  serializeJson(doc, f);
  f.close();
}

void loadSchedules() {
  if (!LittleFS.exists("/schedules.json")) {
    scheduleConfig.count = 0;
    return;
  }

  File f = LittleFS.open("/schedules.json", "r");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, f);
  f.close();

  scheduleConfig.count = 0;

  for (JsonObject s : doc["schedules"].as<JsonArray>()) {
    scheduleConfig.items[scheduleConfig.count].hour   = s["hour"];
    scheduleConfig.items[scheduleConfig.count].minute = s["minute"];
    scheduleConfig.items[scheduleConfig.count].turnOn = s["turnOn"];
    scheduleConfig.items[scheduleConfig.count].active = s["active"];
    scheduleConfig.count++;
  }
}

void addSchedule(int hour, int minute, bool turnOn) {
  int i = scheduleConfig.count++;

  scheduleConfig.items[i].hour = hour;
  scheduleConfig.items[i].minute = minute;
  scheduleConfig.items[i].turnOn = turnOn;
  scheduleConfig.items[i].active = true;

  saveSchedules();
}

void deleteSchedule(int index) {
  if (index < 0 || index >= scheduleConfig.count) return;

  for (int i = index; i < scheduleConfig.count - 1; i++)
    scheduleConfig.items[i] = scheduleConfig.items[i + 1];

  scheduleConfig.count--;
  saveSchedules();
}

String getSchedulesJson() {
  DynamicJsonDocument doc(1024);
  JsonArray arr = doc.createNestedArray("schedules");

  for (int i = 0; i < scheduleConfig.count; i++) {
    JsonObject s = arr.createNestedObject();
    s["hour"]   = scheduleConfig.items[i].hour;
    s["minute"] = scheduleConfig.items[i].minute;
    s["turnOn"] = scheduleConfig.items[i].turnOn;
    s["active"] = scheduleConfig.items[i].active;
  }

  String out;
  serializeJson(doc, out);
  return out;
}
