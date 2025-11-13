The following AC Remote Controller is works as following:
- Upon first time boot, the device will be sending info into the serial with Baud 115200. Connect to the Wifi AP of the ESP32 and navigate to the IP given to connect setup the wifi connection.
- The device will restart and you may now use the controller.


Required hardwares:
- ESP32 or NodeMCU V1.0 (ESP8266MOD)
- IR Transmitter

Install the following Libraries in your arduino:
- ArduinoJson
- ESPAsyncTCP
- ESPAsyncWebServer
- IRremoteESP8266

Connection to be made as per diagram. If you need to have a stronger signal, you can use 2N700 MOSFET instead. Make sure your IR transmitter has resistor connected to it to prevent overload. 

<img width="1755" height="674" alt="ESP8266_IR_Transmitter_bb" src="https://github.com/user-attachments/assets/28a5407a-a50f-426b-9e4c-186accc2f771" />

