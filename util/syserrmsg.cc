#include "syserrmsg.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
//--------------------------------------------------------
namespace kuafu {
namespace detail {
void safe_strerror_r(int err, char *buf, size_t len) {
  if (buf == NULL || len <= 0) {
    return;
  }

#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
  int old_errno = errno;
  int result = strerror_r(err, buf, len);
  if (0 == result) {
    buf[len - 1] = '\0';
  } else {
    int strerror_error;
    int new_errno = errno;
    if (new_errno != old_errno) {
      strerror_error = new_errno;
    } else {
      strerror_error = result;
    }
    snprintf(buf,
             len,
             "Error %d while retrieving error %d",
             strerror_error,
             err);
  }
  errno = old_errno;
#else
  char *result = strerror_r(err, buf, len);
  if (result != buf) {
    // glibc did not use buf and returned a static string instead. Copy it into buf.
    buf[0] = '\0';
    strncat(buf, result, len - 1);
  }
#endif
}
}//namespace detail

std::string SafeSysErrMsgGetter::SafeGetSysErrMsg(int err) {
  constexpr int buffer_size = 256;
  char buf[buffer_size];
  kuafu::detail::safe_strerror_r(err, buf, sizeof(buf));
  return std::string(buf);
}
}//namespace kuafu
