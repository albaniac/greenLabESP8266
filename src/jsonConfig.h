#include <Arduino.h>

#include "FS.h"

#include <ArduinoJson.h>

struct Plant
{
  int d1_z1;
  int d1_z2;
  int d1_z3;

  int d2_z1;
  int d2_z2;
  int d2_z3;

  int d3_z1;
  int d3_z2;
  int d3_z3;
};

struct Config
{
  String deviceName;
  byte wifiMode;
  String wifiSSID;
  String wifiPass;
  byte plantState;
  int plantDay;
  String plantName;
  Plant plant;
};

/*
  wifiMode:
    0 - Off
    1 - AP (ssid = wifiSSID, pass = wifiPass)
    2 - STA (ssid = wifiSSID, pass = wifiPass)
*/

bool saveConfig(const Config &config)
{
  File file = SPIFFS.open("/config.txt", "w");
  // if(file)
  //   file.close();
  // else
  //   Serial.println("Creact error");
  //   return false;

  // File file2 = SPIFFS.open("/config.txt", "w");
  DynamicJsonBuffer jsonBuffer;
  DynamicJsonBuffer jsonBuffer2;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["deviceName"] = config.deviceName;
  root["wifiMode"] = config.wifiMode;
  root["wifiSSID"] = config.wifiSSID;
  root["wifiPass"] = config.wifiPass;
  root["plantState"] = config.plantState;
  root["plantDay"] = config.plantDay;
  root["plantName"] = config.plantName;

  // File file = SPIFFS.open(String("/") + config.plantName + ".txt", "w");
  // JsonObject &plantStruct = jsonBuffer2.createObject();
  // plantStruct["d1_z1"] = config.plant.d1_z1;
  // plantStruct["d1_z2"] = config.plant.d1_z2;
  // plantStruct["d1_z3"] = config.plant.d1_z3;

  // plantStruct["d2_z1"] = config.plant.d2_z1;
  // plantStruct["d2_z2"] = config.plant.d2_z2;
  // plantStruct["d2_z3"] = config.plant.d2_z3;

  // plantStruct["d3_z1"] = config.plant.d3_z1;
  // plantStruct["d3_z2"] = config.plant.d3_z2;
  // plantStruct["d3_z3"] = config.plant.d3_z3;

  // root["plant"] = plantStruct;
  // root["port"] = config.port;
  root.printTo(file);

  file.close();
  return true;
}
bool loadConfig(Config &config)
{
  File configFile = SPIFFS.open("/config.txt", "r");
  if (!configFile)
  {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024)
  {
    Serial.println("Config file size is too large");
    return false;
  }
  String c = configFile.readString();
  Serial.println(c);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(c);
  config.deviceName = String(root["deviceName"].as<String>());
  config.wifiMode = root["wifiMode"];
  config.wifiSSID = String(root["wifiSSID"].as<String>());
  config.wifiPass = String(root["wifiPass"].as<String>());

  config.plantName = String(root["plantName"].as<String>());
  config.plantState = root["plantState"].as<byte>();
  config.plantDay = root["plantDay"].as<int>();
  configFile.close();

  File plantFile = SPIFFS.open(String("/") + config.plantName + ".txt", "r");
  String q = plantFile.readString();
  Serial.println(q);
  DynamicJsonBuffer jsonBuffer2;
  JsonObject &plantJson = jsonBuffer2.parseObject(q);

  config.plant.d1_z1 = plantJson["d1_z1"].as<int>();
  config.plant.d1_z2 = plantJson["d1_z2"].as<int>();
  config.plant.d1_z3 = plantJson["d1_z3"].as<int>();

  config.plant.d2_z1 = plantJson["d2_z1"].as<int>();
  config.plant.d2_z2 = plantJson["d2_z2"].as<int>();
  config.plant.d2_z3 = plantJson["d2_z3"].as<int>();

  config.plant.d3_z1 = plantJson["d3_z1"].as<int>();
  config.plant.d3_z2 = plantJson["d3_z2"].as<int>();
  config.plant.d3_z3 = plantJson["d3_z3"].as<int>();

  plantFile.close();
}