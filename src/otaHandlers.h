#include <ArduinoOTA.h>

void setOtaHandlers() {
  ArduinoOTA.onStart([]() {
    const char* type;
    switch (ArduinoOTA.getCommand()) {
      case U_FLASH:  type = "flash"; break;
      case U_SPIFFS: type = "spiffs filesystem"; break;
      case U_AUTH:   type = "auth"; break;
      default:       type = "unknown";
    }

    Serial.println("\n=== Starting OTA update ===");
    Serial.print("Writing to: ");
    Serial.println(type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update complete, rebooting...");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
}