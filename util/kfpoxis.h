#ifndef KUAFU_POSIX_H_
#define KUAFU_POSIX_H_

namespace kuafu {

#define KF_LIKELY(x) __builtin_expect(!!(x), 1)
#define KF_UNLIKELY(x) __builtin_expect(!!(x), 0)
}

#endif //#ifndef KUAFU_POSIX_H_
