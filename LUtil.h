#ifndef _LUTIL_H_
#define _LUTIL_H_

#if defined(WIN32) || defined(_WIN32) 

#   if defined(_MSC_VER)
#      pragma warning(disable: 4251)  // warning C4251:  class 'std::vector<_Ty>' needs to have dll-interface to be used by clients of class 
#   endif // #if defined(_MSC_VER)

#   ifdef LUTIL_EXPORTS
#      define LUTIL_API __declspec(dllexport)
#   elif defined(USE_TUTIL_DLL)
#      define LUTIL_API __declspec(dllimport)
#   else
#      define LUTIL_API 
#   endif
#else
#   define LUTIL_API 
#endif // #if defined(WIN32) || defined(_WIN32) 

#endif //_LUtil_h_
