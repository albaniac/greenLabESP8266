#include "WifiEsp.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WifiEsp::WifiEsp()
{
}

String WifiEsp::ip2String(const IPAddress &ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

boolean WifiEsp::ap(String ssid, String pass)
{
    IPAddress apIP(192, 168, 4, 1);
    IPAddress staticGateway(192, 168, 4, 1);
    IPAddress staticSubnet(255, 255, 255, 0);
    WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
    WiFi.softAP(ssid.c_str(), pass.c_str());
    WiFi.mode(WIFI_AP);
    String _ip = ip2String(apIP);
    ip = _ip;
    return true;
}

boolean WifiEsp::wifiConnect(byte tries)
{
    if (tries == 0)
        tries = 11;
    while (--tries && WiFi.status() != WL_CONNECTED)
    {

        delay(500);

        delay(500);
    }
    if (WiFi.status() == WL_CONNECTED)
        return true;
    return false;
}

boolean WifiEsp::sta(String ssid, String pass, String host)
{
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);

    WiFi.hostname(host);
    WiFi.begin();
    if (wifiConnect(15))
    {
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
        String _ip = WiFi.localIP().toString();
        ip = _ip;
        return true;
    }

    return false;
}

void WifiEsp::off()
{
    WiFi.mode(WIFI_OFF);
}