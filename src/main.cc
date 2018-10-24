#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Dcnt.h"
#include "CstrHashMap.h"
#include "Logger.h"
#include "SerialLogSink.h"
#include "ErrDefs.h"
#include "TlsLogSink.h"
#include "secrets.h"
#include "otaHandlers.h"
#include "time.h"

const int baudRate = 115200;

// Pins
const byte relayPins[] = {16, 14, 12, 13};
const byte pixelsPin = 2;
const byte btn1Pin = 4;
const byte btn2Pin = 5;

// Pin & Button settings
// const byte fridgePin = relayPins[0];

void setup() {
  // Init Serial
  Serial.begin(baudRate);
  Serial.print("\n\n");
  Serial.println("=== DCNT - Dorm Controller 9000 ===");
  Serial.print("Free sketch space: ");
  Serial.println(ESP.getFreeSketchSpace());

  // Init WiFi
  Serial.println(F("Connecting to WiFi..."));
  Serial.print(F("SSID: "));
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi connection failed. Reboot in 5 seconds...");
    delay(5000);
    ESP.restart();
  }
  WiFi.hostname(F("ESP8266_DCNT"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Init OTA server
  Serial.println(F("Initialising OTA server..."));
  setOtaHandlers();
  ArduinoOTA.setPasswordHash(OTA_PASSWORD_HASH);
  ArduinoOTA.begin();
  Serial.println(F("OTA server started"));

  // Check for OTA update
  Serial.println(F("Checking for OTA update..."));
  for(size_t i = 0; i < 8; i++) {
    delay(250);
    ArduinoOTA.handle();
  }
  Serial.println(F("No OTA update found"));

  // Sync time
  Serial.println(F("Syncing time..."));
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    delay(1000);
  }
  time_t now = time(nullptr);
  Serial.print(F("Time: "));
  Serial.println(now);

  // Init logger
  Serial.println(F("Initialising logger..."));
  Logger<2> logger;
  SerialLogSink serialLogSink;
  logger.registerSink(&serialLogSink);
  TlsLogSink tlsLogSink(IPAddress(10,0,0,11), 5005);
  logger.registerSink(&tlsLogSink);
  logger.log(Sel::INFO, "SETUP", "Logger initialised");

  for (byte i = 0; i < sizeof(relayPins); i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);
  };

  digitalWrite(relayPins[1], HIGH);
  digitalWrite(relayPins[2], HIGH);
  digitalWrite(relayPins[3], HIGH);

  logger.log(Sel::INFO, "SETUP", "Setup complete");
};

void loop() {
  delay(1000);
  Serial.print(".");
  ArduinoOTA.handle();
};
