#ifndef KUAFU_RESTRICT_H_
#define KUAFU_RESTRICT_H_

namespace kuafu {

/*
 * disable copy and move constructor
 */
class NonCopyableForAll {
 protected:
     NonCopyableForAll() = default;
     ~NonCopyableForAll() = default;

     NonCopyableForAll(const NonCopyableForAll&) = delete;
     NonCopyableForAll& operator=(const NonCopyableForAll&) = delete;

     NonCopyableForAll(NonCopyableForAll&&) = delete;
     NonCopyableForAll& operator=(NonCopyableForAll&&) = delete;
};

/*
 * only disable copy constructor
 */
class NonCopyableForCopy {
 protected:
     NonCopyableForCopy() = default;
     ~NonCopyableForCopy() = default;

     NonCopyableForCopy(const NonCopyableForCopy&) = delete;
     NonCopyableForCopy& operator=(const NonCopyableForCopy&) = delete;
};

class StaticUtilClass {
 private:
     StaticUtilClass() = delete;
};

} //namespace kuafu

#endif//KUAFU_RESTRICT_H_
