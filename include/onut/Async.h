#ifndef ASYNC_H_INCLUDED
#define ASYNC_H_INCLUDED

// STL
#include <future>

template<typename ... Targs>
inline auto OAsync(Targs... args) -> decltype(std::async(std::launch::async, args...))
{
    return std::async(std::launch::async, args...);
}

#endif
