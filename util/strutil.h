#ifndef KUAFU_STRUTIL_H_
#define KUAFU_STRUTIL_H_

#include <string>

#include "util/noncoopyable.h"
//------------------------------------------------------
namespace kuafu {

class StrUtil {

 public:
     template<typename StringPtrType>
     static bool NullOrEmpty(StringPtrType strPtr) {
         return !(strPtr && strPtr[0]);
     }

     template<typename StringPtrType>
     static bool NotNullOrEmpty(StringPtrType strPtr) {
         return (strPtr && strPtr[0]);
     }

     static std::string SafeGetString(const char* data) {
         return std::string(nullptr != data ? data : sEmpty);
     }

 public:
     static const std::string sEmpty;

 private:
     StrUtil();
};

}//namespace kuafu

#endif//#ifndef KUAFU_STRUTIL_H_
