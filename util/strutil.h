#ifndef KUAFU_STRUTIL_H_
#define KUAFU_STRUTIL_H_

#include <string>

#include "util/restrict_class.h"
//------------------------------------------------------
namespace kuafu {

class StrUtil : public StaticUtilClass {
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
         return std::string(nullptr != data ? data : empty_);
     }

 public:
     static const std::string empty_;

 private:
     StrUtil();
};

}//namespace kuafu

#endif//#ifndef KUAFU_STRUTIL_H_
