#pragma once
#include <ArduinoJson.h>

struct ScheduleItem {
  int hour;
  int minute;
  bool turnOn;
  bool active;
};

struct ScheduleConfig {
  ScheduleItem items[10];
  int count;
};

extern ScheduleConfig scheduleConfig;

void loadSchedules();
void saveSchedules();
void addSchedule(int hour, int minute, bool turnOn);
void deleteSchedule(int index);
String getSchedulesJson();
