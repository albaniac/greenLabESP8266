#ifndef WifiEsp_h
#define WifiEsp_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif
// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

class WifiEsp{
    public:
        WifiEsp();
        boolean ap(String ssid, String pass);
        void off();
        boolean sta(String ssid, String pass, String host);
        String ip2String(const IPAddress &ipAddress);

        String ip;
    private:
        boolean wifiConnect(byte tries);
        
};
// extern WifiEsp wifiEsp;
#endif