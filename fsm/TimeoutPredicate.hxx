#ifndef LUTIL_TIMEOUTPREDICATE_HXX
#define LUTIL_TIMEOUTPREDICATE_HXX

#include <lutil/LUtil.h>
#include <lutil/fsm/TransitionPredicate.hxx>

#include <boost/scoped_ptr.hpp>

namespace LUtil 
{
class LUTIL_API MachineBase;
class LUTIL_API MachineType;

class LUTIL_API TimeoutPredicate : public IPredicate
{
   public:
      TimeoutPredicate(const MachineType& sourceMachineType);

      virtual bool operator()(const Event* event,
                              const MachineBase& machine) throw();

   private:
      boost::scoped_ptr<MachineType> mSourceMachineType;
};

} // namespace LUtil

#endif // #ifndef LUTIL_TIMEOUTPREDICATE_HXX