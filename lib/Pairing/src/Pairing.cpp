#include <Pairing.h>

#include <vector>

#include <WiFi.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

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

Pairing::Pairing(std::string ssid, std::string password)
    : ssid(ssid), password(password) {
  WiFi.mode(WIFI_MODE_APSTA);
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

void Pairing::begin(std::function<void()> callback) {
  WiFi.softAP(ssid.c_str(), password.c_str());
  delay(200);
  WiFi.softAPConfig(apIP, apIP, mask);

  dnsServer.start(DNS_PORT, SERVER_NAME, apIP);

  webServer.onNotFound(
      [this]() { webServer.send(200, "text/html", responseHTML.c_str()); });

  webServer.on(UriBraces("/ssid/{}/password/{}"), [this, callback]() {
    std::string ssid = webServer.pathArg(0).c_str();
    std::string password = webServer.pathArg(1).c_str();

    WiFi.begin(ssid.c_str(), password.c_str());

    WiFi.localIP().toString();

    setTimeout(
        checkWifiConnected,
        [this, &callback]() {
          webServer.send(200, "application/json",
                         "{ \"status\": \"" + WiFi.localIP().toString() +
                             "\" }");
          delay(200);
          callback();
          pairing = false;
          delay(1000);
          disconnect();
        },
        [this]() {
          webServer.send(403, "application/json",
                         "{ \"status\": \"unauthorized\" }");
        },
        timeout);
  });

  webServer.begin(SERVER_PORT);
}

void Pairing::handleConnection() {
  if (pairing) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}

void Pairing::disconnect() {
  webServer.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
}

void Pairing::setConnectionTimeout(int timeout) { this->timeout = timeout; }