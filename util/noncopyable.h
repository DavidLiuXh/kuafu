#ifndef KUAFU_NONCOPYABLE_H_
#define KUAFU_NONCOPYABLE_H_

namespace kuafu {

/*
 * disable copy and move constructor
 */
class NonCopyableForAll {
 public:
     NonCopyableForAll() {}

     NonCopyableForAll(const NonCopyableForAll&) = delete;
     NonCopyableForAll& operator=(const NonCopyableForAll&) = delete;

     NonCopyableForAll(NonCopyableForAll&&) = delete;
     NonCopyableForAll& operator=(NonCopyableForAll&&) = delete;
};

/*
 * only disable copy constructor
 */
class NonCopyableForCopy {
 public:
     NonCopyableForCopy() {}

     NonCopyableForCopy(const NonCopyableForCopy&) = delete;
     NonCopyableForCopy& operator=(const NonCopyableForCopy&) = delete;
};

class StaticUtilClass {
 private:
     StaticUtilClass();

};

} //namespace kuafu

#endif//KUAFU_NONCOPYABLE_H_
