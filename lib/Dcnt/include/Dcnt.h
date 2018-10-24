#include "Ubokeh.h"

#include "UhServer.h"

using byte = unsigned char;

/**
 * 
 */
class Dcnt {
private:
  class memberLamda;
  Ubokeh::UhServer server;

public:
  Dcnt(unsigned int ok);
  ~Dcnt();
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