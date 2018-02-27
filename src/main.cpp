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

void saveData()
{
  server.send(200, "text/plain", "OK");
  String body = String(server.arg("body"));
  Serial.println(body);
  config.plantName = body;
  config.plantDay = 0;
  loadPlantConfig(config);
  saveConfig(config);
  showConfig(config);
}

void getPlantName()
{
  String cpn = String(config.plantName);
  Serial.println(cpn);
  server.send(200, "text/plain", cpn);
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
  }

  loadConfig(config);

  showConfig(config);
  updateWifi();
  fsb.init((&server));
  server.on("/saveData", HTTP_POST, saveData);
  server.on("/plantName", HTTP_GET, getPlantName);

  server.begin();
  // wifiEsp.ap(ssid, pass);
}
void loop()
{
  server.handleClient();
}
