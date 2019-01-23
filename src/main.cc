#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Dcnt.h"
#include "CstrHashMap.h"
#include "Logger.h"
#include "SerialLogSink.h"
#include "Euhm.h"
#include "StlsLogSink.h"
#include "secrets.h"
#include "otaHandlers.h"
#include "Time.h"
#include "time.h"
#include "Ubokeh.h"
#include "UhServer.h"

const int baudRate = 115200;

// Pins
const byte relayPins[] = {16, 14, 12, 13};
const byte pixelsPin = 2;
const byte btn1Pin = 4;
const byte btn2Pin = 5;

// Pin & Button settings
// const byte fridgePin = relayPins[0];

Dcnt* pDcnt;

void initSerial() {
  Serial.begin(baudRate);
  Serial.print("\n\n");
  Serial.println(F("=== DCNT - Dorm Controller 9000 ==="));
  Serial.print(F("Free sketch space: "));
  Serial.println(ESP.getFreeSketchSpace());
}

void initWiFi() {
  Serial.println(F("* Connecting to WiFi..."));
  Serial.print(F("SSID: "));
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(F("DCNT_ESP8266"));
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(F("WiFi connection failed. Reboot in 5 seconds..."));
    delay(5000);
    ESP.restart();
  }
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("+ Connected to WiFi"));
}

void initOtaUpdateServer() {
  Serial.println(F("* Starting OTA update server..."));
  setOtaHandlers();
  ArduinoOTA.setPasswordHash(OTA_PASSWORD_HASH);
  ArduinoOTA.begin();

  Serial.print(F("Checking for OTA update"));
  for(size_t i = 0; i < 16; i++) {
    Serial.print(".");
    delay(250);
    ArduinoOTA.handle();
  }
  Serial.println(F("\nNo OTA update found"));
  Serial.println(F("+ Started OTA update server"));
}

void syncTime() {
  Serial.println(F("* Syncing time..."));
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    delay(1000);
  }
  time_t now = time(nullptr);
  Serial.printf("Time: %02i:%02i:%02i GMT\n", hour(now), minute(now), second(now));
  Serial.println(F("+ Synced time"));
}

void setup() {
  initSerial();
  initWiFi();
  initOtaUpdateServer();
  syncTime();

  Serial.println(F("=== Bootstrap complete ==="));

  pDcnt = new Dcnt();
  pDcnt->init();

  return;
};

void loop() {
  delay(1000);
  Serial.print(".");
  ArduinoOTA.handle();
};
