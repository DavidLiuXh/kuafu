#ifndef LUTIL_STRUTIL_HXX
#define LUTIL_STRUTIL_HXX

#include <lutil/LUtil.h>

#include <boost/noncopyable.hpp>

#include <string>
//------------------------------------------------------
namespace LUtil
{
   class LUTIL_API StrUtil
   {
   public:
      template<typename StringPtrType>
      static bool NullOrEmpty(StringPtrType strPtr)
      {
         return !(strPtr && strPtr[0]);
      }

      template<typename StringPtrType>
      static bool NotNullOrEmpty(StringPtrType strPtr)
      {
         return (strPtr && strPtr[0]);
      }
      static std::string safeGetString(const char* data)
      {
         return std::string(NULL != data ? data : sEmpty);
      }

      static std::string generateUUIDStr(bool useHash = false);

   public:
      static const std::string sEmpty;
#ifdef _WIN32
      static const std::wstring sWEmpty;
#endif
   private:
      StrUtil();
   };
}//namespace LUtil

#endif//#ifndef LUTIL_STRUTIL_HXX