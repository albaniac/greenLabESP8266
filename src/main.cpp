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
#include <Wire.h>
// #include "U8glib.h"

#include <ArduinoJson.h>
#include "StringCommand.h"
#include "WifiEsp.h"
#include "jsonConfig.h"
#include "FSBrowser.h"

#include <SoftwareSerial.h>

SoftwareSerial swSer(4, 5, false, 256); //rx D2, tx D1

boolean ssp = false;
unsigned long pt = 0;

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
  config.plantState = 1;
  loadPlantConfig(config);
  saveConfig(config);
  showConfig(config);
  swSer.print("&40@");
  delay(2);
  swSer.print(String("^") + config.wifiSSID + "@");
  delay(2);
  swSer.print(config.plantName + " " + config.plant.d1_z1 + "@");
  swSer.print("$start");
  pt = millis();
  ssp = true;
}

void getPlantName()
{
  String cpn = String(config.plantName);
  Serial.println(cpn);
  server.send(200, "text/plain", cpn);
}

void stopPlant()
{
  // String cpn = String();
  // Serial.println(cpn);
  config.plantState = 0;
  config.plantName = "stop";
  saveConfig(config);
  swSer.print("&40@");
  delay(2);
  swSer.print(String("^") + config.wifiSSID + "@");
  delay(2);
  swSer.print("   **Stop**@");

  server.send(200, "text/plain", "OK");
  swSer.print("$stop");
}



void setup()
{
  Serial.begin(115200);
  swSer.begin(115200);
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
  server.on("/stopPlant", HTTP_POST, stopPlant);
  server.on("/plantName", HTTP_GET, getPlantName);

  server.begin();
  swSer.print("&40@");
  delay(2);
  swSer.print(String("^") + config.wifiSSID + "@");
  delay(2);
  if (config.plantState == 1)
  {
    swSer.print(config.plantName + "@");
  }
  else
  {
    swSer.print("   **Stop**@");
  }
  // wifiEsp.ap(ssid, pass);
  pt = millis();
}

// unsigned long pt = 0;
boolean ssp2 = false;
void loop()
{
  unsigned long nt = millis();
  if (config.plantState == 1)
  {
    if (nt - pt > 6000 && ssp == true)
    {
      swSer.print("$on1");
      delay(5);
      swSer.print("$on2");
      delay(5);
      swSer.print("$on3");
      ssp2 = true;
      ssp = false;
      pt = nt;
    }
    if (nt - pt > 6000 && ssp2 == true)
    {
      swSer.print("$off1");
      delay(5);
      swSer.print("$off2");
      delay(5);
      swSer.print("$off3");
      ssp2 = false;
      ssp = false;
      pt = nt;
    }
  }

  // u8g.firstPage();
  // draw();
  server.handleClient();
}
