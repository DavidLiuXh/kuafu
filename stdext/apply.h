#ifndef KUAFU_STDEXT_APPLY_H_
#define KUAFU_STDEXT_APPLY_H_ 

#include <tuple>
#include <utility>
#include <type_traits>

#include "stdext/index_sequence.h"

namespace kuafu {
namespace stdext{

template<typename Func, typename Tuple, std::size_t... index>
auto HelpeApply(Func&& func, Tuple&& tuple, IndexSequence<index...>) ->
decltype(func(std::get<index>(std::forward<Tuple>(tuple))...)) {
    return func(std::get<index>(std::forward<Tuple>(tuple))...);
}

template<typename Func, typename Tuple>
auto Apply(Func&& func, Tuple&& tuple) ->
    decltype(HelpeApply(std::forward<Func>(func),
                          std::forward<Tuple>(tuple),
                          MakeIndexSequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{})) {
    return HelpeApply(std::forward<Func>(func),
                        std::forward<Tuple>(tuple),
                        MakeIndexSequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}

}//namespace stdext
}//namespace kuafu

#endif //#ifndef KUAFU_STDEXT_APPLY_H_
