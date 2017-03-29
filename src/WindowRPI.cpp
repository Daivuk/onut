#if defined(__unix__)

// Onut
//#include <onut/Renderer.h>
#include <onut/Settings.h>
//#include <onut/Strings.h>
//#include <onut/UIContext.h>

// Internal
#include "WindowRPI.h"

namespace onut
{
    OWindowRef Window::create()
    {
        return std::shared_ptr<Window>(new WindowRPI());
    }

    WindowRPI::WindowRPI()
    {
    }

    WindowRPI::~WindowRPI()
    {
    }

    void WindowRPI::setCaption(const std::string& newName)
    {
    }

    bool WindowRPI::pollEvents()
    {
        return true;
    }
}

#endif
