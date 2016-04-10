#pragma once
// STL
#include <functional>

namespace onut
{
    /**
    This should only be called once
    @param initCallback Called once after the engine has been initialized
    @param updateCallback Called at a fixed 120 times per seconds. Sometimes tho calls can be dropped and run slower if the game performs badly
    @param renderCallback Called once per frame. Draw your stuff in there
    @retun This function never returns until the game shutsdown.
    */
    void run(std::function<void()> initCallback, 
             std::function<void()> updateCallback, 
             std::function<void()> renderCallback, 
             std::function<void()> postRenderCallback = nullptr);

    void quit();
}

#define ORun onut::run
#define OQuit onut::quit
