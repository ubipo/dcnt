#include "Ubokeh.h"

#include "UhServer.h"
#include "Logger.h"

using byte = unsigned char;

/**
 * 
 */
class Dcnt {
private:
  class memberLamda;
  Ubokeh::UhServer* pServer = nullptr;
  Sel::Logger<2>* pLogger = nullptr;
  void log(const char* msg);
  const byte relayPins[4] = {16, 14, 12, 13};

  static Sel::Logger<2>* loggerFactory();

public:
  Dcnt();
  ~Dcnt();

  void init();
  bool initialized();

  void test();
  struct LcntReqRes;
  typedef LcntReqRes (*MyLambda)();
  void setSwitch(byte id);
};

// class Lcnt::memberLamda : Ubokeh::ReqHandler {
//   memberLamda();
// };

// struct Lcnt::LcntReqRes {
//   char* errMsg;
// };