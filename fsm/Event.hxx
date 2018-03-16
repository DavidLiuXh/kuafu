#ifndef LUTIL_EVENT_HXX
#define LUTIL_EVENT_HXX

#include <lutil/LUtil.h>

#ifdef USING_USTL
#include <ustl.h>
   using namespace ustl;
#else
#include <ostream>
#include <vector>
   using namespace std;
#endif

namespace LUtil
{
   class LUTIL_API MachineBase;
   class LUTIL_API MachineSet;

   class LUTIL_API Event
   {
      friend class LUTIL_API MachineSet;
      friend class LUTIL_API StateMachine;
      friend class LUTIL_API ActionMachine;
   public:
      Event(const vector<MachineBase*>& targetMachines/* = vector<MachineBase*>()*/) throw();
      Event(MachineBase* targetMachine = NULL) throw();
      Event(MachineSet* machineSet) throw()
         : mMachineSet(machineSet)
      {
      }
      virtual ~Event();
      virtual ostream& toStream(ostream& str) const = 0;
      virtual std::string toString() const = 0;

      const MachineSet* getMachineSet() const { return mMachineSet; }
      MachineSet* getMachineSet() { return mMachineSet; }
      //friend ostream& operator<<(ostream&, const Event& event);

   public: // members
      typedef vector<MachineBase*> MachinePtrList;
      MachinePtrList mTargetMachines;

   private:
      MachineSet* mMachineSet;
   };

   LUTIL_API ostream& operator<<(ostream& str, const Event& event);

} // namespace LUtil

#endif // #ifndef LUTIL_EVENT_HXX
