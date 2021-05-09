#pragma once

#include <functional>
#include <string>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFi.h>

#define DNS_PORT 53
#define SERVER_PORT 80
#define SERVER_NAME "control.mibe.org"

static IPAddress apIP(192, 168, 1, 4);
static IPAddress mask(255, 255, 255, 0);

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