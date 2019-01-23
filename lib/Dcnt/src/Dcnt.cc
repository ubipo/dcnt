#include "Dcnt.h"
#include <Arduino.h>
#include "Logger.h"
#include "SerialLogSink.h"
#include "StlsLogSink.h"
#include "UhServer.h"

Sel::Logger<2>* Dcnt::loggerFactory() {
  Sel::TimeGetter glambda = []() { return 5; };
  Sel::Logger<2>* pLogger = new Sel::Logger<2>(glambda);
  SerialLogSink* pSerialLogSink = new SerialLogSink();
  pLogger->registerSink(pSerialLogSink);
  StlsLogSink* pStlsLogSink = new StlsLogSink(IPAddress(10,0,0,4), 5005);
  pLogger->registerSink(pStlsLogSink);
  return pLogger;
}

void Dcnt::log(const char* msg) {
  if (initialized()) {
    pLogger->info("DCNT", msg);
  }
}

Dcnt::Dcnt() {
}

void Dcnt::init() {
  if (!initialized()) {
    pLogger = loggerFactory();
    Sel::Logger<2>* pLocalLogger = pLogger;

    pServer = new UhServer(5, [pLocalLogger](Ubokeh::Levels level, const char* src, const char* msg) {
      pLocalLogger->log((Sel::Levels)level, src, msg);
    });

    for (byte i = 0; i < sizeof(relayPins); i++) {
      pinMode(relayPins[i], OUTPUT);
      digitalWrite(relayPins[i], LOW);
    };

    digitalWrite(relayPins[1], HIGH);
    digitalWrite(relayPins[2], HIGH);
    digitalWrite(relayPins[3], HIGH);

    log("Init complete");
  }
}

bool Dcnt::initialized() {
  return pServer != nullptr;
}

Dcnt::~Dcnt() {
  if (initialized()) {
    delete pServer;
    delete pLogger;
  }
}

void Dcnt::test() {
  log("YASS bich");
}