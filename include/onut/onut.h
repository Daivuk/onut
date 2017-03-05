#ifndef ONUT_H_INCLUDED
#define ONUT_H_INCLUDED

#include <functional>
#include <string>
#include <vector>

namespace onut
{
    void run(std::function<void()> initCallback,
             std::function<void()> updateCallback,
             std::function<void()> renderCallback,
             std::function<void()> postRenderCallback);
    void quit();
}

extern std::vector<std::string> OArguments;

#define ORun onut::run
#define OQuit onut::quit

#endif
