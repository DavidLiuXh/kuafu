#ifndef LUTIL_TRANSITIONPREDICATE_HXX
#define LUTIL_TRANSITIONPREDICATE_HXX

#include <lutil/LUtil.h>

namespace LUtil
{

class LUTIL_API Event;
class LUTIL_API MachineType;
class LUTIL_API MachineBase;

class LUTIL_API IPredicate
{
   public:
      virtual ~IPredicate();
      virtual bool operator() (const Event*,
                              const MachineBase& machine) throw() = 0;
};

// 
template <typename EventType>
class SimplePredicate : public IPredicate
{
public:
   typedef typename EventType::Type EventEnum;

   SimplePredicate(EventEnum type) throw()
      : mType(type)
   {}

   virtual bool operator() (const Event* event,
      const MachineBase& machine) throw()
   {
      const EventType* levent = dynamic_cast<const EventType*>(event);
      return (levent && levent->getType() == mType);
   }

private:
   EventEnum mType;
};

} // namespace LUtil

#endif // #ifndef LUTIL_TRANSITIONPREDICATE_HXX