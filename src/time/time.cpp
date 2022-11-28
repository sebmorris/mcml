#include <iostream>
#include <chrono>
#include <vector>

using namespace std::chrono;

auto timeFuncInvocation = 
    [](auto&& func, auto&&... params) {
        // get time before function invocation
        const auto& start = high_resolution_clock::now();
        // function invocation using perfect forwarding
        for(auto i = 0; i < 1e5/*largeNumber*/; ++i) {
            std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
        }
        // get time after function invocation
        const auto& stop = high_resolution_clock::now();
        return (stop - start)/1e5/*largeNumber*/;
    };