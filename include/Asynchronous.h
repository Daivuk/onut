#pragma once
#include <future>

/**
    Run a task asynchronously from the current thread.
    @param args Function and arguments, or your usual lambda
    @return std::future<> of the type of your functions
    @note This is the same as calling std::async(std::launch::async, ...). It's just shorter and use the default async launch policy.
*/
template<typename ... Targs>
inline auto OAsync(Targs... args) -> decltype(std::async(args...))
{
    return std::async(std::launch::async, args...);
}
