#ifndef FSBrowser_h
#define FSBrowser_h

#if defined(WIRING) && WIRING >= 100
#include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#define DBG_OUTPUT_PORT Serial

class FSBrowser
{
  public:
    FSBrowser();
    void init(ESP8266WebServer *server);
    String formatBytes(size_t bytes);
    String getContentType(String filename);
    bool handleFileRead(String path);
    void handleFileUpload();
    void handleFileDelete();
    void handleFileCreate();
    void handleFileList();

  private:
    ESP8266WebServer *_server;
    File fsUploadFile;
};
// extern WifiEsp wifiEsp;
#endif