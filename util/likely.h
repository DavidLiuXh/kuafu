#ifndef KUAFU_LIKELY_H_
#define KUAFU_LIKELY_H_

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif // #ifndef KUAFU_LIKELY_H_
