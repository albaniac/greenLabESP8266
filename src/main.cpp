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
// U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // I2C / TWI
// void draw(void)
// {
//   // graphic commands to redraw the complete screen should be placed here
//   u8g.setFont(u8g_font_unifont);
//   //u8g.setFont(u8g_font_osb21);
//   u8g.drawStr(0, 22, "Hello World!");
// }

// void setup(void)
// {
// flip screen, if required
// u8g.setRot180();

// set SPI backup if required
//u8g.setHardwareBackup(u8g_backup_avr_spi);

// assign default color value

// pinMode(8, OUTPUT);
// }

// void loop(void)
// {
//   // picture loop
//   u8g.firstPage();
//   do
//   {
//     draw();
//   } while (u8g.nextPage());

//   // rebuild the picture after some delay
//   //delay(50);
// }

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
  // Wire.pins(5, 4);

  // if (u8g.getMode() == U8G_MODE_R3G3B2)
  // {
  //   u8g.setColorIndex(255); // white
  // }
  // else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  // {
  //   u8g.setColorIndex(3); // max intensity
  // }
  // else if (u8g.getMode() == U8G_MODE_BW)
  // {
  //   u8g.setColorIndex(1); // pixel on
  // }
  // else if (u8g.getMode() == U8G_MODE_HICOLOR)
  // {
  //   u8g.setHiColorByRGB(255, 255, 255);
  // }

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
  else{
    swSer.print("   **Stop**@");
  }
  // wifiEsp.ap(ssid, pass);
}
void loop()
{
  // u8g.firstPage();
  // draw();
  server.handleClient();
}
