#include <string>

#include "util/restrict_class.h"
//--------------------------------------------------------
namespace kuafu {

class SafeSysErrMsgGetter : public StaticUtilClass {
  public:
    static std::string SafeGetSysErrMsg(int err);
};//class SafeSysErrMsgGetter
}//namespace kuafu
