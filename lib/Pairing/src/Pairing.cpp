#include <Pairing.h>

#include <vector>

#include <WiFi.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

Pairing::Pairing(std::string ssid, std::string password)
    : ssid(ssid), password(password) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid.c_str(), password.c_str());
  delay(100);

  WiFi.softAPConfig(apIP, apIP, mask);

  dnsServer.start(DNS_PORT, SERVER_NAME, apIP);
}

void Pairing::setTimeout(std::function<bool()> handler,
                         std::function<void()> resolve,
                         std::function<void()> reject, int timeout) {
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();

  bool connected = false;

  do {
    currentMillis = millis();
    connected = !handler();

    if (currentMillis - previousMillis >= timeout) {
      reject();
      break;
    }

  } while (!connected);

  if (connected)
    resolve();
}

bool Pairing::checkWifiConnected() { return WiFi.status() != WL_CONNECTED; }

void Pairing::onPair(std::function<void()> handler) {
  bool pairing = true;

  webServer.onNotFound(
      [this]() { webServer.send(200, "text/html", responseHTML.c_str()); });

  webServer.on(UriBraces("/ssid/{}/password/{}"), [this, &handler, &pairing]() {
    std::string ssid = webServer.pathArg(0).c_str();
    std::string password = webServer.pathArg(1).c_str();

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    this->setTimeout(
        checkWifiConnected,
        [this, &pairing, &handler]() {
          webServer.send(200, "application/json",
                         "{ \"status\": \"connected\" }");
          handler();
          webServer.stop();
          dnsServer.stop();
          pairing = false;
        },
        [this]() {
          webServer.send(404, "application/json",
                         "{ \"status\": \"invalid\" }");
        },
        5000);
  });

  webServer.begin(SERVER_PORT);

  while (pairing) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}