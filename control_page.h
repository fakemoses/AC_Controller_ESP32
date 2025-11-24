#pragma once
#include <ESPAsyncWebServer.h>
#include "IRac.h"
#include "config.h"
#include "schedule.h"

extern IRac ac;

const char control_html[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head>
<title>AC Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
button{width:80px;height:40px;margin:5px;}
.container { max-width: 400px; margin:auto; font-family: Arial; }
input { padding:8px; margin:3px; width:80px; }
.schedule-item { margin:5px 0; padding:5px; border:1px solid #ccc; }
</style>
</head><body>
<div class="container">

<h2>AC Control</h2>
<a href="/on"><button>ON</button></a>
<a href="/off"><button>OFF</button></a>

<hr>

<h3>Schedules</h3>

<div id="schedule-list">Loading...</div>

<hr>

<h3>Add Schedule</h3>
<form id="addForm">
  <label>Hour:</label><input type="number" id="hour" min="0" max="23"><br>
  <label>Minute:</label><input type="number" id="minute" min="0" max="59"><br>
  <label>Action:</label>
  <select id="action">
    <option value="1">Turn ON</option>
    <option value="0">Turn OFF</option>
  </select><br>
  <button type="submit">Add</button>
</form>

</div>

<script>
// Load schedules on page load
function loadSchedules() {
  fetch('/schedules')
    .then(r => r.json())
    .then(data => {
      const list = document.getElementById('schedule-list');
      list.innerHTML = '';

      if (!data.schedules || data.schedules.length === 0) {
        list.innerHTML = '<p>No schedules set.</p>';
        return;
      }

      data.schedules.forEach((s, idx) => {
        const div = document.createElement('div');
        div.className = 'schedule-item';

        div.innerHTML = `
          <strong>${s.hour.toString().padStart(2,'0')}:${s.minute.toString().padStart(2,'0')}</strong>
          - ${s.turnOn ? 'ON' : 'OFF'}
          <button onclick="deleteSchedule(${idx})" style="width:60px;height:30px;">Del</button>
        `;

        list.appendChild(div);
      });
    });
}

// Add schedule
document.getElementById('addForm').addEventListener('submit', function(e) {
  e.preventDefault();

  const hour = document.getElementById('hour').value;
  const minute = document.getElementById('minute').value;
  const action = document.getElementById('action').value;

  fetch(`/schedule/add?hour=${hour}&minute=${minute}&turnOn=${action}`, {
    method: 'POST'
  }).then(() => loadSchedules());
});

// Delete schedule
function deleteSchedule(index) {
  fetch(`/schedule/delete?index=${index}`, { method: 'POST' })
    .then(() => loadSchedules());
}

// Init
loadSchedules();
</script>

</body></html>
)HTML";

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

  //// scheduler api
  server.on("/schedules", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", getSchedulesJson());
  });

  server.on("/schedule/add", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (!request->hasParam("hour", true) || !request->hasParam("minute", true) || !request->hasParam("turnOn", true)) {
        request->send(400, "text/plain", "Missing parameters");
        return;
      }

      int hour = request->getParam("hour", true)->value().toInt();
      int minute = request->getParam("minute", true)->value().toInt();
      bool turnOn = request->getParam("turnOn", true)->value() == "1";

      addSchedule(hour, minute, turnOn);
      request->send(200, "text/plain", "Schedule added");
  });

  server.on("/schedule/delete", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (!request->hasParam("index", true)) {
        request->send(400, "text/plain", "Missing index");
        return;
      }

      int index = request->getParam("index", true)->value().toInt();
      deleteSchedule(index);
      request->send(200, "text/plain", "Schedule deleted");
  });
}
