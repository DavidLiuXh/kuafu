#ifndef KUAFU_STDEXT_INDEX_SEQUENCE_H_
#define KUAFU_STDEXT_INDEX_SEQUENCE_H_

#include <cstddef>

namespace kuafu {
namespace stdext{

// Impletation make_index_sequence template class in the c++14
template<std::size_t... N>
struct IndexSequence {
};

template<std::size_t N, std::size_t... M>
struct MakeIndexSequence
:public MakeIndexSequence<N-1, N - 1, M...> {
};

template<std::size_t... M>
struct MakeIndexSequence<0, M...>
:public IndexSequence<M...> {
};

}//namespace stdext
}//namespace kuafu

#endif //#ifndef KUAFU_STDEXT_INDEX_SEQUENCE_H_
