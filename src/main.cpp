/*
 * Green Lab
 */
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"
#include <ESP8266HTTPUpdateServer.h>

#include <ArduinoJson.h>
#include "StringCommand.h"
#include "WifiEsp.h"
#include "jsonConfig.h"
#include "FSBrowser.h"

/*
  wifiMode:
    0 - Off
    1 - AP (ssid = wifiSSID, pass = wifiPass)
    2 - STA (ssid = wifiSSID, pass = wifiPass)
*/
const char *filename = "/config.json";
Config config;

WifiEsp wifie = WifiEsp();

// WifiEsp wifi = new WifiEsp();
ESP8266WebServer server(80);
FSBrowser fsb;

void updateWifi()
{
  switch (config.wifiMode)
  {
  case 0:
    wifie.off();
    break;
  case 1:
    wifie.ap(config.wifiSSID, config.wifiPass);
    break;
  case 2:
    wifie.sta(config.wifiSSID, config.wifiPass, config.deviceName);
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s", fileName.c_str());
    }
    Serial.printf("\n");
  }+

  
  loadConfig(config);
  Serial.println(config.deviceName);
  Serial.println(config.wifiMode);
  Serial.println(config.wifiSSID);
  Serial.println(config.wifiPass);
  Serial.println(config.plantName);
  Serial.println(config.plant.d1_z1);
  
  updateWifi();
  fsb.init((&server));
  server.on("/uploadConfig", HTTP_POST, [](){
    server.send(200, "OK";)
  });
  
  server.begin();
  // wifiEsp.ap(ssid, pass);
}
void loop()
{
  server.handleClient();
}
