#pragma once

#include <functional>
#include <string>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

#define DNS_PORT 53
#define SERVER_PORT 80
#define SERVER_NAME "conrtol.mibe.org"

static IPAddress apIP(192, 168, 1, 4);
static IPAddress mask(255, 255, 255, 0);

static std::string responseHTML =
    "<!DOCTYPE html><html>"
    "<head><meta name=\"viewport\" content=\"width=device-width, "
    "initial-scale=1\">"
    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px "
    "auto; text-align: center;}"
    "</style></head>"
    "<body><h1>ESP32 Web Server</h1>"
    "<p>Hello World</p>"
    "</body></html>";

class Pairing {
public:
  Pairing(std::string ssid, std::string password);

  void setConnectionTimeout(int timeout);

  void begin(std::function<void()> callback);
    void disconnect();
    void handleConnection();

private:
  void setTimeout(std::function<bool()> handler, std::function<void()> resolve,
                  std::function<void()> reject, int timeout);

  static bool checkWifiConnected();

private:
  std::string ssid;
  std::string password;

  int timeout = 5000;

  DNSServer dnsServer;
  WebServer webServer;
};