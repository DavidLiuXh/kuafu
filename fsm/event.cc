#include "fsm/event.h"

namespace kuafu {

std::ostream& operator<<(std::ostream& str, const Event& event) {
   return event.ToStream(str);
}

} //namespace kuafu
